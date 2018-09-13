#include "process.h"


static int nextPid = 1; //Esta variable le asigna a cada proceso un pid distinto

tProcess* createProcess(char* processName,void* startingPoint, int parentPid, int argc, char* argv[]){
  /*Se reserva espacio para la estructura del proceso*/
  tProcess* process = mallocMemory(sizeof(tProcess));

  /*Se completan los campos del nuevo proceso*/
  process->pid = nextPid++;
  process->parentPid = parentPid;
  //process->name = duplicateString(processName); //lo comente pq no estaba reconociendome strlen aunque ponia include string.h
  process->processMemoryLowerAddress = mallocMemory(PROCESS_SIZE);
  void* processMemoryUpperAddress = process->processMemoryLowerAddress + PROCESS_SIZE -1;
  process->stackPointer = initializeStack(processMemoryUpperAddress - sizeof(tStackFrame) +1 , argc, argv, startingPoint);
  process->state = READY;
return process;
}


//RowDaBoat
void* initializeStack(void* stackPointer, int argc, char* argv[], void* startingPoint){
  tStackFrame* stackFrame = (tStackFrame*) stackPointer;
  stackFrame->gs= 0x001;
  stackFrame->fs= 0x002;
  stackFrame->r15= 0x003;
  stackFrame->r14= 0x004;
  stackFrame->r13= 0x005;
  stackFrame->r12= 0x006;
  stackFrame->r11= 0x007;
  stackFrame->r10= 0x008;
  stackFrame->r9=	0x009;
  stackFrame->r8= 0x00A;
  stackFrame->rsi= (uint64_t)argv;
  stackFrame->rdi= (uint64_t)argc;
  stackFrame->rbp= 0x00D;
 stackFrame->rdx= 0x00;
  stackFrame->rcx= 0x00F;
  stackFrame->rbx= 0x010;
  stackFrame->rax= 0x011;
  stackFrame->rip= (uint64_t)startingPoint;
  stackFrame->cs=	0x008;
  stackFrame->eflags= 0x202;
  stackFrame->rsp= (uint64_t)&(stackFrame->base); //el rsp apunta a la direccion en la que esta el base
  stackFrame->ss= 0x000;
  stackFrame->base=	0x000;
return stackFrame;
}



void deleteProcess(tProcess* process){
  freeMemory(process->name);
  freeMemory(process->processMemoryLowerAddress);
  freeMemory(process);
}
