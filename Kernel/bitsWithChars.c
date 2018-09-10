#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include "bitsWithChars.h"

unsigned char * newBuffer(int bitsAmmount) {
    if(bitsAmmount < 8)
        bitsAmmount = 8;
    unsigned char * buffer;
    buffer = calloc((bitsAmmount + 7) / 8, sizeof(char));
    return buffer;
}

void set(int index, char bitValue, unsigned char * buffer) {
    while(index > 7) {
        buffer++;
        index -= 8;
    }
    index = 7 - index;
    if((*buffer & (unsigned char) pow(2,index)) == 0) {
        if(bitValue > 0)
            *buffer += (unsigned char) pow(2, index);
    }
    else {
        if( bitValue == 0)
            *buffer -= (unsigned char) pow(2,index);
    }
}

unsigned char get(int index, unsigned char * buffer) {
    while(index > 7) {
        buffer++;
        index -= 8;
    }
    index = 7 - index;
    return (*buffer & (unsigned char) pow(2,index)) > 0 ? 1 : 0;
}