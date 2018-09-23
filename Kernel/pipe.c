#include <mutexAndSemaphore.h>
#include <stdint.h>
#include <stdlib.h>
#include <BuddyAllocationSystem.h>
#define BUFFERSIZE 1024

/*
 * LEER!!!!!!!!!!!!!!
 * En nuestro tp no llamamos a ninguna funcion cuando termina el programa como para cerrar el pipe
 * no se si hay que hacerlo
 */

typedef struct pipe_t{
    char * name;
    mutex_t * useMutex;
    mutex_t * readMutex;
    mutex_t * writeMutex;
    char buffer[BUFFERSIZE];
    unsigned int readPosition;
    unsigned int writePosition;
} pipe_t;

typedef struct pipeNode{
    pipe_t * pipe;
    struct pipeNode * next;
}pipeNode;

void unlinkPipe(char * name);
pipeNode * unlinkPipeR(pipeNode * node, char * name);
pipe_t * pipe(char * name);
int readPipe(pipe_t * pipe, char * resp, size_t ammount);
int writePipe(pipe_t * pipe, char * msg, size_t ammount);
int getAvailableToWrite(int start, int final, int size);
char * strconcat(char * s1, char * s2);
int strcmp(char s1[], char s2[] );
int strlen(char * s);
char * strcpy(char * s);

pipeNode * pipeList = NULL;

pipe_t * pipe(char * name) {
    if (name == NULL)
        return NULL;

    pipe_t * newPipe = mallocMemory(sizeof(pipe_t));
    if(newPipe == NULL)
        return NULL;

    newPipe->name = strcpy(name);
    newPipe->readMutex = mutex_open(strconcat(name, " read"));
    newPipe->writeMutex = mutex_open(strconcat(name, " write"));
    newPipe->useMutex = mutex_open(strconcat(name, " use"));
    newPipe->readPosition = 0;
    newPipe->writePosition = 0;

    //OJO
    post(newPipe->readMutex);

    pipeNode * newNode = mallocMemory(sizeof(pipeNode));

    if(newNode == NULL)
        return NULL;

    newNode->pipe = newPipe;
    newNode->next = pipeList;
    pipeList = newNode;

    return newPipe;
}

void unlinkPipe(char * name) {
    pipeList = unlinkPipeR(pipeList, name);
}

pipeNode * unlinkPipeR(pipeNode * node, char * name) {
    if(node->pipe == NULL)
        return NULL;
    if(strcmp(node->pipe->name, name) == 0) {
        pipeNode * aux = node->next;
        freeMemory(node->pipe->name);
        unlink(node->pipe->useMutex);
        unlink(node->pipe->readMutex);
        unlink(node->pipe->writeMutex);
        freeMemory(node->pipe);
        return aux;
    }
    node->next = unlinkPipeR(node->next, name);
    return node;
}

int readPipe(pipe_t * pipe, char * resp, size_t ammount) {

    if(ammount > BUFFERSIZE)
        ammount = BUFFERSIZE;

    wait(pipe->readMutex);
    while(BUFFERSIZE - getAvailableToWrite(pipe->readPosition, pipe->writePosition, BUFFERSIZE) <= 0) {
        /*
         * ACA HACEN WAITCONDVAR, DESPUES VEMOS
         */
    }


    wait(pipe->useMutex);
    int availableToRead = BUFFERSIZE - getAvailableToWrite(pipe->writePosition, pipe->readPosition, BUFFERSIZE);
    int i;
    for(i = 0; i < availableToRead ; i++, (pipe->readPosition)++) {
        if(pipe->readPosition == 1024)
            pipe->readPosition = 0;
        resp[i] = (pipe->buffer)[pipe->readPosition];
    }
    post(pipe->useMutex);

    post(pipe->readMutex);
    return i;
}

int writePipe(pipe_t * pipe, char * msg, size_t ammount){

    if(ammount > BUFFERSIZE)
        ammount = BUFFERSIZE;

    wait(pipe->writeMutex);
    while(getAvailableToWrite(pipe->readPosition, pipe->writePosition, BUFFERSIZE) < ammount)
        /*
         * ACA HACEN WAITCONDVAR, DESPUES VEMOS
         */

    wait(pipe->useMutex);
    for(int i = 0; i < ammount; i++, (pipe->writePosition)++) {
        if(pipe->writePosition == 1024)
            pipe->writePosition = 0;
        (pipe->buffer)[pipe->writePosition] = msg[i];
    }
    post(pipe->useMutex);

    post(pipe->writeMutex);
    return ammount;
}

int getAvailableToWrite(int start, int final, int size) {
    if(final >= start)
        return size - (final - start);
    else {
        return start - final;
    }
}


//YA ESTA COMO PARA HACER UN STRING.h
int strcmp(char s1[], char s2[] ) {
    for (int i = 0; ; i++) {
        if (s1[i] != s2[i]) {
            return s1[i] < s2[i] ? -1 : 1;
        }

        if (s1[i] == '\0') {
            return 0;
        }
    }
}

char * strconcat(char * s1, char * s2) {
    int s1lenght= strlen(s1);
    int s2lenght = strlen(s2);
    char * resp = mallocMemory(strlen(s1) + strlen(s2) + 1);
    for(int i = 0, j = 0; i <= s1lenght+s2lenght; i++,j++) {
        if(i < s1lenght) {
            resp[i] = s1[j];
            if(i + 1 >= s1lenght)
                j = -1;
        }
        else {
            resp[i] = s2[j];
        }
    }
    return resp;
}

int strlen(char * s) {
    int i;
    for(i = 0; s[i] != '\0'; i++) {
    }
    return i;
}

char * strcpy(char * s) {
    char * resp = mallocMemory(strlen(s) + 1);
    for(int i = 0; s[i-1] != 0; i++) {
        resp[i] = s[i];
    }
    return resp;
}