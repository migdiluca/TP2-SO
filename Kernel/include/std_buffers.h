#include <stdint.h>

#ifndef STD_BUFFERS_H
#define STD_BUFFERS_H

#define STD_OUT 1
#define STD_IN 0
#define STD_ERR 2

//Inicializa los buffers de entrada y salida.
void setUpBuffers();
//Limpia el buffer pedido por el índice que se pasa como parametro.
void clearBuffer(int index);
//Escribe un caracter en el buffer seleccionado.
int writeCharBuffer(int index, char character);
//Escribe un entero en el buffer seleccionado en la base pasada como parámetro.
int writeIntBuffer(int index, uint64_t value,int base);
//Escribe un string en el buffer seleccionado.
int writeStrBuffer(int index, char * str);
//Lee el primer caracter que se escribió en el buffer solicitado y luego lo borra.
char readCharBuffer(int index);
//Carga en el string pasado como parámetro el buffer seleccionado hasta que llegue a size.
int readStrBuffer(int index, char * str,int size);

#endif
