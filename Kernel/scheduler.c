#define N_PROCESSES 20

#include "process.h"

typedef struct processNode{
  tProcess* process;
  struct processNode * next;
}tProcessNode;

typedef struct{
  tProcessNode* first;
  tProcessNode* last;
  int processAmount;
}tProcessQueue;


static tProcessQueue processQueue;

int removeProcessFromQueue(tProcess* process);
tProcessNode* removeProcessFromQueueR(tProcessNode* current , int pid);
int addProcessToQueue(tProcess* process);
void startScheduler();





void startScheduler(){
  processQueue.processAmount =0;
}






int addProcessToQueue(tProcess* process){
  tProcessNode* aux = mallocMemory(sizeof(tProcessNode));
  if(aux == NULL){
    return -1;
  }
  aux->next = NULL;
  if(processQueue.processAmount == 0){
    processQueue.first = aux;
    processQueue.last = processQueue.first;
    (processQueue.first)->process = process;
  }
  else{
    (processQueue.last)->next = aux;
    (processQueue.last) = (processQueue.last)->next;
  }
  processQueue.processAmount++;
  return 0; // si necesitamos el pid, devolvemos el pid
}








int removeProcessFromQueue(tProcess* process){
  if(processQueue.processAmount <= 0 || process->pid < 0){
    return -1;
  }
  processQueue.first = removeProcessFromQueueR(processQueue.first, process->pid);
  return 0;
}


tProcessNode* removeProcessFromQueueR(tProcessNode* current , int pid){
  if(current == NULL){
    return current;
  }

  if(current->next->next == NULL && current->next->process->pid == pid){ //significa que habia que borrar el ultimo
    processQueue.last = current;
    deleteProcess(current->next->process);
    freeMemory(current->next);
    processQueue.processAmount--;
    return current;
  }


  if(current->process->pid == pid){
    tProcessNode* aux = current->next;
    deleteProcess(current->process);
    freeMemory(current);
    processQueue.processAmount--;
    return aux;
  }

  current->next = removeProcessFromQueueR(current->next, pid);

return current;
}
