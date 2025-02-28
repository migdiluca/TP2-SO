#include <stdint.h>
#include "videoDriver.h"
#include "syscallDispatcher.h"
#include "rtc.h"
#include "std_buffers.h"
#include <lib.h>
#include <soundDriver.h>
#include "BuddyAllocationSystem.h"
#include "process.h"
#include "scheduler.h"

typedef uint64_t(*systemCall)();

systemCall sysCalls[] = { 0, 0, 0,
    (systemCall) _writePixel,
    (systemCall) _readPixel,
    (systemCall) _getScreenWidth,
    (systemCall) _getScreenHeight,
    (systemCall) _write,
    (systemCall) _read,
    (systemCall) _clearBuffer,
    (systemCall) _readTime,
    (systemCall) _scrollScreen,
    (systemCall) _replaceColor,
    (systemCall) _backupScreen,
    (systemCall) _restoreScreen,
    (systemCall) _fillScreen,
    (systemCall) _writeBlock,
    (systemCall) _beep, 0, //ESTE 0 ES DE UNBEEP. PREGUNTAR Q ONDA
    (systemCall) _createProcess,
    (systemCall) _endProcess,
    (systemCall) _blockProcess,
    (systemCall) _unBlockProcess,
    (systemCall) _mallocMemory,
    (systemCall) _callocMemory,
    (systemCall) _reallocMemory,
    (systemCall) _freeMemory,
    (systemCall) _exec,
    (systemCall) _kill,
    (systemCall) _killCurrentProcess,
    (systemCall) _psProcess,

  //(systemCall) _activateBeep,
  //(systemCall) _deactivateBeep
};


void syscallDispatcher(uint64_t index, uint64_t a, uint64_t b, uint64_t c, uint64_t d, uint64_t e){
  sysCalls[index](a, b, c, d , e);
}

void _beep(uint64_t timer){
  beep(timer);
}

// void _activatebeep(){
//   activateBeep();
// }
//
// void _deactivateBeep(){
//   deactivateBeep();
// }

void _writePixel(uint64_t width, uint64_t height, struct RGB color){
  writePixel(width, height, color);
}

void _writeBlock(uint64_t width, uint64_t height, struct RGB color, uint64_t sizeX,uint64_t sizeY) {
  writeBlock(width,height,color,sizeX,sizeY);
}

void _readPixel(uint64_t width, uint64_t height, struct RGB * color){
  *color = readPixel(width,height);
}

uint64_t _getScreenWidth(){
  return getWidth();
}

uint64_t _getScreenHeight(){
  return getHeight();
}

void _write(uint64_t buffer, char c){
  writeCharBuffer(buffer, c);
}

uint64_t _read(uint64_t buffer){
  return (uint64_t)readCharBuffer(buffer);
}

uint64_t _clearBuffer(uint64_t index){
  clearBuffer(index);
  return 0;
}

uint64_t _readTime(uint64_t time) {
  if(time == 0)
    return getSeconds();
  if(time == 1)
    return getMinutes();
  if(time == 2)
    return getHour();
  return -1; //si me pasaron un parametro invalido
}

void _scrollScreen(uint64_t ammount, struct RGB background) {
  movePixelsUp(ammount, background);
}

void _replaceColor(struct RGB colorOld, struct RGB colorNew){
  replaceColor(colorOld,colorNew);
}

void _backupScreen() {
  backupScreen();
}

void _restoreScreen(){
  restoreScreen();
}

void _fillScreen(struct RGB color) {
  fillScreen(color);
}

uint64_t _createProcess(char * processName, void * startingPoint, int argc, char* argv[]) {
    tProcess * proc = createProcess(processName, startingPoint, 0, argc, argv);
    addProcess(proc);
    return proc->pid;
}

void _endProcess() {
    //endProcess();
}

void _blockProcess(uint64_t pid) {
    blockProcess(pid);
}

void _unBlockProcess(uint64_t pid) {
    unblockProcess(pid);
}

uint64_t _mallocMemory(uint64_t size, uint64_t address) {
    return (uint64_t) mallocMemoryInProcess(size, getRunningProcess());
}

uint64_t _callocMemory(uint64_t size) {
    return callocMemory(size);
}

uint64_t _reallocMemory(uint64_t addr, uint64_t size) {
    return reallocMemory(addr, size);
}

void _freeMemory(uint64_t addr) {
    freeMemoryInProcess(addr, getRunningProcess());
}

void _exec(void* startingPoint, void* pid, int cargs, void ** pargs){
    int* p = (int*)pid;
    tProcess* process = createProcess("default", startingPoint, 0, cargs, pargs);//volar parent pid
    addProcess(process);
    *p = process->pid;
}


void _kill(int pid, int message){
  switch(message){
    case 0:
      endProcess(pid);
    break;
    case 1:
      blockProcess(pid);
    break;
    case 2:
      unblockProcess(pid);
    break;
  }
}

void _killCurrentProcess(){
  endProcess(getRunningPid());
}

void _psProcess(char* buffer, int size){
  sprintProcesses(buffer, size);
}
