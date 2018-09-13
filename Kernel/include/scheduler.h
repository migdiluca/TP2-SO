#ifndef _defs_
#define _defs_
#include "process.h"

#define MAX_PROCESSES 20



typedef struct processNode{
  tProcess* process;
  struct processNode * next;
}tProcessNode;

typedef struct{
  tProcessNode* first;
  tProcessNode* last;
  int processAmount;
}tProcessQueue;

int removeProcessFromQueue(tProcess* process);
tProcessNode* removeProcessFromQueueR(tProcessNode* current , int pid);
int addProcessToQueue(tProcess* process);
void startScheduler();



#endif
