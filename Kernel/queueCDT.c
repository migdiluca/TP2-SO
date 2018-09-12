#include "queueADT.h"
typedef struct TNode {
    struct TNode * next;
    void * elem;
} TNode;


typedef struct queueCDT {
    size_t bytes;
    int (*cmp)(void *, void *);
    TNode * last;
    TNode * first;
} queueCDT;

static TNode * removeREC(TNode * current, TNode * prev, void * elem, queueADT q);
static int belongsREC(TNode * node, void * elem, int (*cmp)(void *, void *));

queueADT newQueue(size_t bytes, int (*cmp)(void *, void *)) {
    queueADT new = mallocMemory(sizeof(*new));
    new->bytes = bytes;
    new->first = new->last = NULL;
    new->cmp = cmp;
    return new;
}

void push(queueADT q, void * elem) {
    if (q == NULL) {
        return;
    }
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

void * pop(queueADT q) {
    if (q == NULL) {
        return NULL;
    }
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

static
TNode * removeREC(TNode * current, TNode * prev, void * elem, queueADT q) {
    if (current == NULL) {
        return NULL;
    } if (!(*q->cmp)(current->elem, elem)) {
        if (current == q->last) {
            q->last = prev;
        }
        return current->next;
    } else {
        current->next = belongsREC(current->next, elem, q);
        return current;
    }
}

void removeElem(queueADT q, void * elem) {
    if (q == NULL) {
        return;
    }
    if (q->cmp == NULL) {
        return;
    }
    q->first = removeREC(q->first, NULL, elem, q);
}

static
int belongsREC(TNode * node, void * elem, int (*cmp)(void *, void *)) {
    if (node == NULL) { // llego al final y no lo encontro
        return 0;
    } if (!(*cmp)(node->elem, elem)) { // si son iguales entonces lo encontre
        return 1;
    } else
        return 0 + belongsREC(node->next, elem, cmp);
}

int belongsElem(queueADT q, void * elem) {
    if (q == NULL) {
        return 0;
    }
    if (q->cmp == NULL) {
        return 0;
    }
    return belongsREC(q->first, elem, q->cmp);
}

void freeQueue(queueADT q) {
    if (q == NULL) {
        return;
    }
    TNode * queue = q->first;
    while (queue != NULL) {
        TNode * aux = queue;
        queue = queue->next;
        freeMemory(aux->elem);
        freeMemory(aux);
    }
    freeMemory(q);
}
