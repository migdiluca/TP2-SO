//
//  mutex.h
//  TPARQUI
//
//  Created by Fermin Gomez on 9/10/18.
//
//

#ifndef sync_h
#define sync_h

#include <stdio.h>
#include "interrupts.h"
#include "queueADT.h"

typedef struct mutex {
    char value;
    int mutex_holder;
    queueADT queue;
} mutex;

typedef struct semaphore {
    char value;
    queueADT queue;
} semaphore;

mutex * initMutex(int value);
void acquire(mutex * m);
void release(mutex * m);

semaphore * initSem();
void wait(semaphore * s);
void signal(semaphore * s);

#endif /* mutex_h */
