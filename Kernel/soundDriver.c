#include <soundDriver.h>
#include <time.h>
#include <beep.h>

void beep(int timer){
  activateBeep();
  newTimer(timer, deactivateBeep);
}

void activateBeep(){
  beepasm();
}

void deactivateBeep(){
  unbeepasm();
}
