
#ifndef INTERRUPTS_H
#define INTERRUPTS_H

//Handlers de los diferentes IRQ.
void _irq00Handler(void);
void _irq01Handler(void);
void _irq02Handler(void);
void _irq03Handler(void);
void _irq04Handler(void);
void _irq05Handler(void);

//Devuelve la posición donde empezar el stack cuando corre el código de user.
void * getStackPointer();

//Handler de los diferentes syscalls
void _syscallHandler(uint64_t,uint64_t,uint64_t,uint64_t,uint64_t);

//Handler de las excepciones.
void _exception0Handler(void);
void _exception6Handler(void);

//Inhabilita las interrupciones
void _cli(void);
//Habilita las interrupciones
void _sti(void);
//Detiene el procesador.
void _hlt(void);

//Setea las mascaras de los PIC.
void picMasterMask(uint8_t mask);
void picSlaveMask(uint8_t mask);

//Termina la ejecución de la cpu.
void haltcpu(void);


#endif
