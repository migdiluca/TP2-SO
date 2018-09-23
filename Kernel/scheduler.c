#include "scheduler.h"
#include "String.h"

char buff[8];
//static tProcessNode * running;
//static int amount = 0;

static queueADT ready; // colas de procesos en espera para ser ejecutados
static queueADT blocked; // cola de procesos bloqueados
static tProcess * running = NULL; // puntero al proceso que se esta corriendo en este momento

void probandoEscribirEnKernel2() {
    int i = 0;
    while (i < 100) {
        writeString(" Cosa ");
        i++;
    }
    writeString(" Me Bloqueo ");
    blockProcess(getRunningPid());
    i = 0;
    while (i < 100) {
        writeString(" Cosa ");
        i++;
    }
    endProcess(getRunningPid());
}

void probandoEscribirEnKernel5() {
    int i = 0;
    while (i < 100) {
        writeString("My pid:  ");
        uintToBase(getRunningPid(), buff, 10);
        writeString(buff);
        writeString("\n");
        i++;
    }
    endProcess(getRunningPid());
}

void probandoEscribirEnKernel4() {
    tProcess * aux = createProcess("Imprime", probandoEscribirEnKernel5, 0, 0, NULL);
    push(ready, aux);
    int i = 0;
    while (i < 1000) {
        writeString("Hola");
        i++;
    }
    endProcess(getRunningPid());
}

void probandoEscribirEnKernel3() {
    int i = 0;
    while (i < 500) {
        writeString(" Vamo ");
        i++;
    }
    unblockProcess(3);
    endProcess(getRunningPid());
}
void probandoEscribirEnKernel() {
    int i = 0;
    while (i <1500) {
        writeString(" Genere ");
        i++;
    }
    dumpMemory();
    while(1) {
        writeString(" Genere ");
    }
    endProcess(getRunningPid());
}

int getRunningPid() {
    return (running == NULL ) ? -1 : running->pid;
}

void* getRunningProcess(){
  return running;
}

void addProcess(tProcess * p) {
    push(ready, p);
}

void blockProcess(int pid) {
    _cli();
    changeProcessState(pid, WAITING);
    _hlt();
}

void unblockProcess(int pid) {
    _cli();
    writeString("Volvi");
    tProcess * elem = mallocMemory(sizeof(tProcess));
    elem->pid = pid;
    tProcess * aux = removeElem(blocked, elem);
    if (aux == NULL) {
        _sti();
        return;
    }
    freeMemory(elem);
    aux->state = READY;
    push(ready, aux);
    _sti();
}

changeProcessState(int pid, pState state) {
    if (running->pid == pid) {
        running->state = state;

        writeString("\n");
        writeString("RunningDead Pid:");
        uintToBase(getRunningPid(), buff, 10);
        writeString(buff);
        writeString("\n");

        return;
    }
    //TODO: esta parte del codigo jamas fue testada, nose si se usara en algun caso mucho sentido no tiene
    tProcess * elem = mallocMemory(sizeof(tProcess));
    elem->pid = pid;
    tProcess * aux = removeElem(ready, elem);
    if (aux == NULL) {
        aux = removeElem(blocked, elem);
        freeMemory(elem);
        aux->state = state;
        push(blocked, aux);
        return;
    }
    freeMemory(elem);
    aux->state = state;
    push(ready, aux);
}

//void runNextProcess() { // yield
//    if (running != NULL) {
//        addProcess(running);
//    }
//    running = pop(ready); // que pasa si ready esta vacia??
//    contextSwitch(running->stackPointer);
//}


void runNextProcess() {
    //    _hlt();
    scheduler();
    contextSwitch(running->stackPointer);
}

//void scheduler() {
//    push(ready, running);
//    running->state = READY;
//    running = pop(ready);
//    running->state = RUNNING;
//}

void scheduler() {
    if (running->state == DEAD) {
        deleteProcess(running);
    } else if (running->state == RUNNING) {
        running->state = READY;
        push(ready, running);
    } else if (running->state == WAITING) {
        push(blocked, running);
    }
    while ((running = pop(ready))->state != READY) {
        if (running->state == DEAD) {
            deleteProcess(running);
        } else if (running->state == WAITING) {
            push(blocked, running);
        }
    }
    running->state = RUNNING;
}

void * dispatcher(int rsp) {
    if (running == NULL) {
        return rsp;
    }
    running->stackPointer = rsp;
    scheduler();
    return running->stackPointer;
}

/**
 Funcion que compara dos rocesos p1, p2 segun sus pids retorna (> 0) si p1 > p2 (< 0) caso contrario 0 si son iguales
 */
int cmpProcess(tProcess * p1, tProcess * p2) {
    return (p1->pid) - (p2->pid);
}

void init_Process() {
    tProcess * proc = createProcess("maite capa", probandoEscribirEnKernel, 0, 0, NULL);
    printProcess(proc);

    tProcess * anotherP = createProcess("fer0", probandoEscribirEnKernel2, 0, 0, NULL);
    printProcess(anotherP);

    tProcess * anotherP1 = createProcess("fer1", probandoEscribirEnKernel3, 0, 0, NULL);
    printProcess(anotherP1);

    dumpMemory();

    push(ready, proc);
    push(ready, anotherP);
    push(ready, anotherP1);

    probandoEscribirEnKernel4();
    //endProcess();
}

void init_(void * startingPoint) {
    ready = newQueue(sizeof(tProcess), cmpProcess);
    blocked = newQueue(sizeof(tProcess), cmpProcess);
    running = createProcess("theGodFather", startingPoint, 0, 0, NULL);
    running->state = RUNNING;
    printProcess(running);
    contextSwitch(running->stackPointer);
}

//void * dispatcher(int rsp) {
//    if (running != NULL) {
//        running->stackPointer = rsp;
//        push(ready, running);
//    }
//    running = pop(ready);
//    if (running != NULL) {
//        return running->stackPointer;
//    }
//    return rsp;
//}

//void * dispatcher(int rsp) {
//    if (running == NULL) {
//        running = pop(ready);
//    } else {
//        running->stackPointer = rsp;
//        running = pop(ready);
//    }
//    return running->stackPointer;
//}

//void addProcess(tProcess * process) {
//    tProcessNode * node = mallocMemory(sizeof(tProcessNode));
//    node->process = process;
//    if (running == NULL) {
//        running = node;
//        running->next = running;
//    } else {
//        node->next = running->next;
//        running->next = node;
//    }
//    amount++;
//}
//
////void removeProcess(int pid) {
////    if (running == NULL) {
////        return;
////    } else if(cmpProcess(running->process, running->next->process) && running->process->pid == pid) {
////        deleteProcess(running->process);
////        freeMemory(running);
////        running = NULL;
////        amount--;
////        return;
////    }
////    tProcessNode * prev = running;
////    tProcessNode * current = running->next;
////    while (current->process->pid != pid) {
////        prev = current;
////        current = current->next;
////    }
////    if (cmpProcess(current->process, running->process)) {
////        running = running->next;
////    }
////    prev->next = current->next;
////    amount--;
////    addToFreeQueue(slotToRemove);
////    deleteProcess(current->process);
////    freeMemory(current);
////}
//
//
//void getNextProcess() {
//    if (running->process->state = RUNNING) {
//        running->process->state = READY;
//    }
//    running = running->next;
//    while (running->process->state != READY) {
//        running = running->next;
//    }
//    running->process->state = RUNNING;
//}
//
//void * dispatcher(int rsp) {
//    if (running == NULL) {
//        return rsp;
//    }
//    running->process->stackPointer = rsp;
//    getNextProcess();
//    return running->process->stackPointer;
//}
//

void sprintProcesses(char* buffer, int buffSize){
    TNode* aux;
    int index = 0;
    int occ;
    char pid[0];
    int s;
    char* states[4];
    states[0] = "ready";
    states[1] = "running";
    states[2] = "waiting";
    states[3] = "dead";

     intToString(pid, running->pid);
     occ = strcpy2(buffer+index,pid,buffSize);
     index += occ;
     buffSize -= occ;
      occ = strcpy2(buffer+index,"     ",buffSize);
      index += occ;
     buffSize -= occ;

     s = stateIdentifier(running->state);
     occ = strcpy2(buffer+index,states[s],buffSize);
     index+=occ;
     buffSize-=occ;


    if(ready != NULL){
        aux = ready->first;
        while(aux!= NULL){
          tProcess* p = aux->elem;
          intToString(pid, p->pid);
          occ = strcpy2(buffer + index, pid, buffSize);
          index += occ;
          buffSize -= occ;
          if(buffSize<=0) break;
           occ = strcpy2(buffer+index,"     ",buffSize);
           index+=occ;
           buffSize-=occ;
           if(buffSize<=0) break;
           s = stateIdentifier(p->state);
           occ = strcpy2(buffer+index,states[s],buffSize);
           index+=occ;
           buffSize-=occ;
           if(buffSize<=0) break;
           aux = aux->next;
        }

  }

     if(blocked != NULL){
         aux = blocked->first;
         while(aux!= NULL){
           tProcess* p = aux->elem;
           intToString(pid, p->pid);
           occ = strcpy2(buffer + index, pid, buffSize);
           index += occ;
           buffSize -= occ;
           if(buffSize<=0) break;
            occ = strcpy2(buffer+index,"     ",buffSize);
            index+=occ;
            buffSize-=occ;
            if(buffSize<=0) break;
            s = stateIdentifier(p->state);
            occ = strcpy2(buffer+index,states[s],buffSize);
            index+=occ;
            buffSize-=occ;
            if(buffSize<=0) break;
            aux = aux->next;
         }
   }


}
