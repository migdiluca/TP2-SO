#ifndef TIME_H
#define TIME_H

//Crea un timer que recibe como primer parametro su duracion y como segundo un
//puntero a funcion que responde como callback.
void newTimer(int, void(*)());

//Funcion que se ejecuta cuando ocurre una interrupcion de hardware de tipo timer tick.
void timer_handler();

//Cantidad de ticks que pasaron desde que se inicio el sistema.
int ticks_elapsed();

//Cantidad de segundos que pasaron desde que se inciio el sistema.
int seconds_elapsed();

#endif
