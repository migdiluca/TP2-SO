//
//  BuddyAllocationSystem.c
//  TPARQUI
//
//  Created by Fermin Gomez on 9/3/18.
//
#include "BuddyAllocationSystem.h"

#define TRUE 1
#define FALSE 0

#define BUCKET_AMOUNT 25
#define LEAF_SIZE  32

#define total_size (1<<BUCKET_AMOUNT) * LEAF_SIZE
#define size_of_level(n) (total_size / (1<<(n)))
#define max_blocks_of_level(n)  ((1<<(n)))
#define is_leaf_block(n) ((1 << (BUCKET_AMOUNT - 1)) <= (n)) ? TRUE : FALSE
#define move_to_parent(index) (((index) - 1) / 2)
#define move_to_left_child(index) ((index) * 2 + 1)
#define move_to_right_child(index) ((index) * 2 + 2)
#define move_to_sibling(index) ((((index) - 1) ^ 1) + 1)

typedef struct list_t {
    struct list_t * next, * prev;
} list_t;

static uint8_t * baseAdress;
extern uint8_t endOfKernel;
static list_t * buckets[BUCKET_AMOUNT];
static int splitBlocks[(1 << (BUCKET_AMOUNT - 1))];
static int allocatedBlocks[1 << (BUCKET_AMOUNT)];


int indexInLevelOf(uint8_t * memoryAdr, int level) {
    return((memoryAdr - baseAdress) >> (29 - level)) + (1 << level) - 1;
}

int adress(int index, int level) {
    return baseAdress + ((index - (1 << level) + 1) << (29 - level));
}

void init() {
    baseAdress = (uint8_t *)&endOfKernel;
    list_t * node = (list_t *)baseAdress;
    node->next = NULL;
    buckets[0] = node;
    for (int i = 0; i < (1 << (BUCKET_AMOUNT - 1)); i++) {
        splitBlocks[i] = FALSE;
    } for (int i = 0;  i < 1 << (BUCKET_AMOUNT); i++) {
        allocatedBlocks[i] = FALSE;
    }
}

int bucketRequest(size_t request) {
    int bucket = BUCKET_AMOUNT - 1;
    int size = LEAF_SIZE;
    while (size < request) {
        bucket--;
        size *= 2;
    }
    return bucket;
}

list_t * popList(int level) {
    list_t * aux = buckets[level];
    buckets[level] = aux->next;
    buckets[level]->prev = NULL;
    return aux;
}

void pushList(int level, uint8_t * memoryAdr) {
    list_t * node = (list_t *)memoryAdr;
    node->next = buckets[level];
    node->prev = NULL;
    buckets[level]->prev = node;
    buckets[level] = node;
}

void removeFromList(int level, list_t * node) {
    if (node == buckets[level]) { // es el primer elemento de la lista
        popList(level);
        return;
    }
    list_t * next = node->next;
    list_t * prev = node->prev;
    if (prev != NULL) {
        prev->next = next;
    } if ( next != NULL) {
        next->prev = prev;
    }
    
}

int blockHasBeenSplit(uint8_t * memoryAdr, int level) {
    int index = indexInLevelOf(memoryAdr, level);
    return splitBlocks[index];
}

int findLevel(uint8_t * memoryAdr) {
    int n = BUCKET_AMOUNT- 1;
    while (n > 0) {
        if (blockHasBeenSplit(memoryAdr, (n-1))) {
            return n;
        }
        n = n - 1;
    }
    return 0;
}

int isParentSplit(int index) {
    return splitBlocks[move_to_parent(index)];
}

void flipParentSplit(int index) {
    splitBlocks[move_to_parent(index)] = !splitBlocks[move_to_parent(index)];
}

void * mallocMemory(size_t request) {
    if (request > size_of_level(0)) {
        return NULL;
    }
    if (baseAdress == NULL) {
        init();
    }
    int level = bucketRequest(request);
    if (!getNextFreeBlock(level)) {
        return NULL;
    }
    list_t * list = popList(level);
    int index = indexInLevelOf((uint8_t *)list, level);
    allocatedBlocks[index] = TRUE;
    return (void *)list;
}

int getNextFreeBlock(int level) {
    int i = level;
    while ((buckets[i] == NULL) && (i >=0)) {
        i--;
    }
    if (i < 0) {
        return FALSE;
    }
    while (i < level) {
        splitBlock(i++);
    }
    return TRUE;
}

void splitBlock(int level) {
    list_t * aux = popList(level);
    int index = indexInLevelOf(aux, level);
    splitBlocks[index] = TRUE;
    pushList(level+1, adress(move_to_right_child(index), level+1));
    pushList(level+1, adress(move_to_left_child(index), level+1));
}

void freeMemory(void * memoryAdr) {
    if (memoryAdr == NULL) {
        return;
    }
    int level = findLevel(memoryAdr);
    int index = indexInLevelOf(memoryAdr, level);
    allocatedBlocks[index] = FALSE;
    while (index > 0) {
        if (allocatedBlocks[move_to_sibling(index)]) {
            break;
        }
        removeFromList(level, (list_t *)adress(move_to_sibling(index), level));
        index = move_to_parent(index);
        splitBlocks[index] = FALSE;
        level--;
    }
    pushList(level, adress(index, level));
}

void * reallocMemory(void * memorySrc, size_t request) {
    uint8_t * aux = (uint8_t *)memorySrc;
    uint8_t * memoryAdr = mallocMemory(request);
    int level = findLevel(aux);
    int size = size_of_level(level);
    for (int i = 0; i < size; i++) {
         *(memoryAdr+i) = *(aux+i);
    }
    freeMemory(memorySrc);
    return memoryAdr;
}

void * callocMemory(size_t request) {
    uint8_t * memoryAdr = (uint8_t *)mallocMemory(request);
    for (int i = 0; i < request; i++) {
        *(memoryAdr+i) = 0;
    }
    return memoryAdr;
}

void dumpMemory() {
//    int index = 0;
//    int level = 0;
//    int maxIndex = 0;
//    while (level < BUCKET_AMOUNT) {
//        maxIndex += max_blocks_of_level(level);
//        putStr("Level: ", colour);
//        uintToBase(level, buff, 10);
//        putStr(buff, colour);
//        putChar('\n', colour);
//        while (index < maxIndex) {
//            uintToBase(index, buff, 10);
//            putStr(" -- ", colour);
//            putStr(buff, colour);
//            if (!is_leaf_block(index)) {
//                if (splitBlocks[index]) {
//                    putStr(" is Split", colour);
//                    index++;
//                    continue;
//                }
//            }
//            if (allocatedBlocks[index]) {
//                putStr(" is Allocated", colour);
//            } else {
//                putStr(" is Free", colour);
//            }
//            index++;
//        }
//        putStr(" --", colour);
//        putChar('\n', colour);
//        level++;
//    }
//    putChar('\n', colour);
}
