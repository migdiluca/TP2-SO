#include <stdio.h>
#include "String.h"

int strcpy2(char* str1, const char* str2, int length);
void intToString(char* str, int num);
void reverseCharArray(char* str, int length);
int numLength(int num);
char* strcpy3(char* destination, const char* source) ;

char* strcpy3(char* destination, const char* source) {
    if (destination == NULL) {
        return NULL;
    }
    char *ptr = destination;
    while (*source) {
        *destination = *source;
        destination++;
        source++;
    }
    *destination = 0;
    return ptr;
}

int strcpy2(char* str1, const char* str2, int length) {
  int i;
  for (i = 0; i < length && str2[i]!='\0'; i++) {
    str1[i] = str2[i];
  }
  if(i!=length) str1[i]='\0';
  return i;
}


void intToString(char* str, int num) {
  int length = numLength(num);
  int numLength = length;
  int i;

  if (num < 0) {
    str[length - 1] = '-';
    num *= -1;
    numLength--;
  }

  for (i = 0; i < numLength; i++) {
    str[i] = (char)(num % 10 + '0');
    num /= 10;
  }

  reverseCharArray(str, length);
  str[length] = '\0';
  return length;
}

void reverseCharArray(char* str, int length) {
  char aux[length];
  int i;
  strcpy2(aux, str, length);

  for (i = 0; i < length; i++) {
    str[length - 1 - i] = aux[i];
  }
}

int numLength(int num) {
  int length = 1;

  if (num < 0) {
    length += 1;
    num *= -1;
  }

  while(num / 10 != 0) {
    length += 1;
    num /= 10;
  }
  return length;
}
