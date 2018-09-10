#include "queueADT.h"

typedef struct TNode {
    struct TNode * next;
    void * elem;
} TNode;

typedef struct queueCDT {
    size_t bytes;
    TNode * last;
    TNode * first;
} queueCDT;


queueADT newQueue(size_t bytes) {
    queueADT new = mallocMemory(sizeof(*new));
    new->bytes = bytes;
    new->first = new->last = NULL;
    return new;
}

void add(queueADT q, void * elem) {
    if (q->first == NULL) {
        TNode * new = mallocMemory(sizeof(*new));
        new->elem = mallocMemory(q->bytes);
        memcpy(new->elem, elem, q->bytes);
        new->next = NULL;
        q->first = q->last = new;
        return;
    } else {
        TNode * new = mallocMemory(sizeof(*new));
        new->elem = mallocMemory(q->bytes);
        memcpy(new->elem, elem, q->bytes);
        new->next = NULL;
        q->last->next = new;
        q->last = new;
        return;
    }
}

void * get(queueADT q) {
    if (q->first == NULL) {
        return NULL;
    }
    TNode * aux = q->first->next;
    void * out = mallocMemory(q->bytes);
    memcpy(out, q->first->elem, q->bytes);
    freeMemory(q->first->elem);
    freeMemory(q->first);
    q->first = aux;
    return out;
}

void freeQueue(queueADT q) {
    TNode * queue = q->first;
    while (queue != NULL) {
        TNode * aux = queue;
        queue = queue->next;
        freeMemory(aux->elem);
        freeMemory(aux);
    }
    freeMemory(q);
}
