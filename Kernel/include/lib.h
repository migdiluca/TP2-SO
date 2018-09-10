#ifndef LIB_H
#define LIB_H

#include <stdint.h>
#include <string.h>
#include "BuddyAllocationSystem.h"

//Estructura para RGB.
struct RGB{
  unsigned char red;
  unsigned char green;
  unsigned char blue;
};

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);
uint64_t uintToBase(uint64_t value, char * buffer, uint64_t base);
char *cpuVendor(char *result);

char* duplicateString(const char* string);

#endif
