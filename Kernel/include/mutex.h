//
//  mutex.h
//  TPARQUI
//
//  Created by Fermin Gomez on 9/10/18.
//
//

#ifndef mutex_h
#define mutex_h

#include <stdio.h>
#include "interrupts.h"
#include "queueADT.h"

typedef struct mutex {
    char value;
    queueADT queue;
} mutex;

mutex * initMutex();
void acquire(mutex * m);
void release(mutex * m);

#endif /* mutex_h */
