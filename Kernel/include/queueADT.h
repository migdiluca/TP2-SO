#ifndef queueADT_h
#define queueADT_h

#include <stdio.h>
#include <stdlib.h>
#include "BuddyAllocationSystem.h"
#include "lib.h"

typedef struct queueCDT * queueADT;

/**
 Crea una queue genericas de elementos de tamañ bytes al no recevir una funcion comparadora no podemos utilizar las funciones remove y belongs */

queueADT newQueue(size_t bytes);

/**
 Creaa una queue genericas de elementos de tamañ bytes y recive una funciona coparadora (con esta queue podemos uitlizar las funciones belongs y remove)
 */
queueADT newQueue(size_t bytes, int (*cmp)(void *, void *));

/**
 Libera los recursos de la queue (q)
 */
void freeQueue(queueADT q);

/**
 Agrega un elemento (* elem) al final de la queue (q) o al principio si esta vacia con shallow copy
 */
void add(queueADT q, void * elem);

/**
 Quita el primer elemento de la queue (q)
 */
void * get(queueADT q);

/**
 Remueve un elemento (* elem) de la queue (q)
Se podra utilizar si es que posee una funcion comparadora
 */
void remove(queueADT q, void * elem);

/**
 Retorna 1 si un elemento (* elem) pertenece a la queue (q) 0 sino.
 Se podra utilizar si es que posee una funcion comparadora
 */
int belongs(queueADT q, void * elem);

#endif