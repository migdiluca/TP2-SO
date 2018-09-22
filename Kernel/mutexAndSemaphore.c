#include <stdint.h>
#include <stdlib.h>
#include <mutexAndSemaphore.h>
#include <scheduler.h>
#include <BuddyAllocationSystem.h>

struct pid_list {
    int pid;
    struct pid_list * next;
};


//SI ES UN MUTEX VALUE = -1 es bloqueado y -2 desbloqueado Y SE TRATA CASI IGUAL QUE UN SEMAFORO
struct sem_t {
    int value;
    int id;
    pid_list * usedBy;
    pid_list * waitListStart;
    pid_list * waitListTail;
    struct sem_t * next;
} ;

sem_t * semList = NULL;
mutex_t * mutexList = NULL;

sem_t * sem_open(char * name, int flag, int initialValue) {
    return semAndMutexOpen(name,flag, initialValue, 1);
}

mutex_t * mutex_open(char * name) {
    return semAndMutexOpen(name,O_CREAT, -2, 0);
}

/**
 * Abre un nuevo semaforo.
 * @param name. Nombre del semaforo.
 * @param flag. O_CREAT crea el semaforo o lo retorna si ya existe, O_EXCL lo crea unicamente si no existe.
 * @param initialValue
 * @return
 */
sem_t * semAndMutexOpen(char * name, int flag, int initialValue, int isSemaphore) {
    if(name == NULL || (isSemaphore && initialValue < 0))
        return NULL;


    int nameID = getStringID(name);
    sem_t * searchedSemOrMutex;
    if(isSemaphore)
        searchedSemOrMutex = getSem(nameID);
    else
        searchedSemOrMutex = getMutex(nameID);

    if(searchedSemOrMutex != NULL) {
        if(flag != O_EXCL)
            return searchedSemOrMutex;
        return NULL;
    }

    sem_t * newSemOrMutex = mallocMemory(sizeof(sem_t));
    if(newSemOrMutex == NULL)
        return NULL;

    newSemOrMutex->id = nameID;
    newSemOrMutex->value = initialValue;
    newSemOrMutex->waitListStart = NULL;
    newSemOrMutex->waitListTail = NULL;

    if(isSemaphore) {
        newSemOrMutex->next = semList;
        semList = newSemOrMutex;
    }
    else {
        newSemOrMutex->next = mutexList;
        mutexList = newSemOrMutex;
    }

    return newSemOrMutex;
}

/**
 * Pregunta si se bloquearia el semaforo al hacer un sem_wait.
 * @param El semaforo a probar.
 * @return -1 si el semaforo no existe.
 *          0 si el proceso no pasaria a estar bloqueado al hacer un sem_wait.
 *          1 si el proceso pasaria a estar bloqueado al hacer un sem_wait.
 */
int trywait(sem_t * sem) {
    if(!exists(sem))
        return -1;
    if(sem->value > 0 || sem->value == -2) {
        return 0;
    }
    return 1;
}

/**
 * Decrementa el valor del semaforo si no hay ningun proceso espearando, en caso contrario agrega el
 * proceso que lo llamo a la lista de espera.
 * @param El semaforo a usar.
 * @return -1 en caso de error, 0 en otro caso.
 */
int wait(sem_t * sem){
    if(!exists(sem))
        return -1;
    if(sem->value > 0) {
        (sem->value)--;
        return 0;
    }
    else if(sem->value == -2) /*si es mutex */{
        (sem->value)++;
        return 0;
    }
    else {
        int currentPid = getRunningPid();
        sem->waitListTail->next = mallocMemory(sizeof(pid_list));
        sem->waitListTail->next->pid = currentPid;
        sem->waitListTail->next->next = NULL;
        if(sem->waitListStart == NULL)
            sem->waitListStart = sem->waitListTail;
        blockProcess(currentPid);
        return 0;
    }
}

/**
 * Incrementa el valor del semaforo si no hay nadie en la lista de espera, si hay alguien lo quita de la
 * lista de espera y el proceso pasa a preparado.
 * @param El semaforo a usar.
 * @return -1 en caso de error, 0 en otro caso.
 */
int post(sem_t * sem) {
    if(!exists(sem))
        return -1;

    pid_list * firstWaiting = sem->waitListStart;
    if(firstWaiting != NULL) {
        unblockProcess(firstWaiting->pid);
        sem->waitListStart = firstWaiting->next;
        if(sem->waitListStart == NULL)
            sem->waitListTail = NULL;
        freeMemory(firstWaiting);
    }
    else if(sem->value > 0)
        (sem->value)++;
    else if(sem->value == -1)
        (sem->value)--;
    //TENNDRIA QUE DEVOLVER -1 SI YA ESTA DESBLOQUEADO EL MUTEX?.
    // NO SE QUE QUISE DECIR ARRIBA DEPUSE VEO
    return 0;
}

int getStringID (char * name) {
    int h = 0;
    for (int i = 0; name[i] != '\0'; i++) {
        h = 31 * h + name[i];
    }
    return h;
}

mutex_t * getMutex(int id) {
    mutex_t * aux = mutexList;
    while(aux != NULL) {
        if(aux->id == id)
            return aux;
        else
            aux = aux->next;
    }
    return NULL;
}

sem_t * getSem(int id){
    sem_t * aux = semList;
    while(aux != NULL) {
        if(aux->id == id)
            return aux;
        else
            aux = aux->next;
    }
    return NULL;
}

pid_list * listRemovePID(pid_list * pidList, int pid) {
    if(pidList == NULL)
        return NULL; //NO SE SI EL CASTEO ES NECESARIO
    if(pidList->pid == pid) {
        pid_list * aux = pidList->next;
        freeMemory(pidList);
        return aux;
    }

    pidList->next = listRemovePID(pidList->next, pid);
    return pidList;
}

void freeMemoryPidLists(pid_list * pidList) {
    if(pidList == NULL)
        return;

    pid_list * aux = pidList;
    while(pidList != NULL) {
        aux = pidList->next;
        freeMemory(pidList);
        pidList = aux;
    }
}

int close(sem_t * sem) {
    if(!exists(sem))
        return -1;
    int pid = getRunningPid();
    sem->usedBy = listRemovePID(sem->usedBy,pid);
    if(sem->usedBy == NULL)
        unlink(sem);
    return 0;
}

int unlink(sem_t * sem) {
    if(!exists(sem))
        return -1;
    freeMemoryPidLists(sem->waitListStart);
    freeMemoryPidLists(sem->usedBy);
    freeMemory(sem);
    return 0;
}

int exists(sem_t * sem) {
    if(sem->value >= 0) {
        if(getSem(sem->id) == NULL)
            return 0;
    }
    else {
        if(getMutex(sem->id) == NULL)
            return 0;
    }
    return 1;
}

