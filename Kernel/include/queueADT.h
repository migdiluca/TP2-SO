#ifndef queueADT_h
#define queueADT_h

#include <stdio.h>
#include <stdlib.h>
#include "BuddyAllocationSystem.h"
#include "lib.h"

typedef struct queueCDT * queueADT;

queueADT newQueue(size_t bytes);

void freeQueue(queueADT q);

void add(queueADT q, void * elem);

void * get(queueADT q);

#endif