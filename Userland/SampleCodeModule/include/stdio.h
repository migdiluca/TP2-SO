#ifndef STDIO_H
#define STDIO_H

#define _stdin 0
#define _stdout 1
#define _stderr 2

//Escribe en el buffer de la salida estandar segun el formato y parametros que reciba.
void printf(char* format, ...);

//Lee de la entrada estandar segun lo indicado en el formato y sus parametros.
int scanf(char* format, ...);

//Escribe un char en la salida estandar
void putchar(char c);

//Obtiene un caracter de la entrada estandar
char getchar();

//Escribe un caracter en el buffer indicado por el segundo parametro (STDIN=0, STDOUT=1, STDERR=2)
void write(char c, int dest);

//Lee del buffer indicado por su parametro (STDIN=0, STDOUT=1, STDERR=2)
char read(int dest);

#endif
