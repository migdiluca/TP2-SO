#ifndef STDLIB_H
#define STDLIB_H

// Compara si dos strings son iguales.
int strcmp( char * str1,  char* str2);
// Recibe un numero y lo convierte a char * con un la base pasada como parametro.
char * intToChar(int value,int base,char * buffer);
// Recibe un char * (minimo con 8 lugares) y escribe el reloj para poder imprimirlo adecuadamente.
void clockString(char * clockTime);

#endif
