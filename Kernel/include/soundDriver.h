

#ifndef SOUND_DRIVER_H
#define SOUND_DRIVER_H

//Reproduce un sonido durante el tiempo especificado por el parametro.
void beep(int);

//Activan y desactivan el speaker. (Wrappers del beep.asm)
void activateBeep();
void deactivateBeep();

#endif
