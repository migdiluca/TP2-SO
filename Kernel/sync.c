//#include "sync.h"
//
///**
// Inicializa el mutex en 1
// TODO: agregar nombre o mecanismo para que pueda ser accedido desde otro proceso
// Retorna puntero al mutex
// */
//mutex * initMutex() {
//    mutex * m;
//    m->queue = newQueue(sizeof(int), NULL);
//    m->value = 1;
//    return m;
//}
//
///**
// El proceso que llama toma el mutex
// si algun otro proceso llama a la funcion y no es el dueño actual del mismo este sera bloqueado
// funcion atomica
// */
//void acquire(mutex * m) {
//    _cli();
//
//    if (m->value == 0) { // esta tomado
//        //push(m->queue, getMyPid());
//        //block(getMyPid());
//    }
//
//    m->value = 0;
//    //m->mutex_holder = getMyPid();
//
//    _sti();
//}
//
///**
// El proceso que es dueño del mutex es el unico que puede liberarlo
// si algun otro proceso llama a la funcion y no es el dueño actual del mismo este no sera liberado
// funcion atomica
// */
//void release(mutex * m) {
//    _cli();
//
////    if (mutex_holder == getMyPid()) {
////        m->value = 1;
////        //wakeUp(*(pop(m->queue)));
////    }
//
//    _sti();
//}
//
///**
// Inicializa el semaforo en el valor dado (int value)
// TODO: agregar nombre o mecanismo para que pueda ser accedido desde otro proceso
//  Retorna puntero al semaforo
// */
//semaphore * initSem(int value) {
//    semaphore * sem;
//    sem->queue = newQueue(sizeof(int), NULL);
//    sem->value = value;
//    return sem;
//}
//
///**
// Incrementa el valor del semaforo
// si el valor del semaforo es menor a cero entonces el proceso que llama a la funcion sera bloqueado hasta que el semaforo sea incrementado mediante signal
// funcion atomica
// */
//void wait(semaphore * s) {
//    _cli();
//
//    s->value--;
//    if (s->value < 0) {
//        //push(s->queue, getMyPid());
//        //block(getMyPid());
//    }
//
//    _sti();
//}
//
//
///**
// Decrementa el valor del semaforo
// funcion atomica
// */
//void signal(semaphore * s) {
//    _cli();
//
//    s->value++;
//    if (s->value <= 0) {
//        //wakeUp(*(pop(s->queue)));
//    }
//
//    _sti();
//}