#include <stdio.h>

int strcomp(char * s1, char * s2 ) {
    for (int i = 0; ; i++) {
        if (s1[i] != s2[i]) {
            return s1[i] < s2[i] ? -1 : 1;
        }

        if (s1[i] == '\0') {
            return 0;
        }
    }
}

char * strconcat(char * s1, char * s2, char * dest) {
    int s1lenght= strlenght(s1);
    int s2lenght = strlenght(s2);
    for(int i = 0, j = 0; i <= s1lenght+s2lenght; i++,j++) {
        if(i < s1lenght) {
            dest[i] = s1[j];
            if(i + 1 >= s1lenght)
                j = -1;
        }
        else {
            dest[i] = s2[j];
        }
    }
    return dest;
}

int strlenght(char * s) {
    int i;
    for(i = 0; s[i] != '\0'; i++) {
    }
    return i;
}

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
  int lenghtNum = length;
  int i;

  if (num < 0) {
    str[length - 1] = '-';
    num *= -1;
      lenghtNum--;
  }

  for (i = 0; i < lenghtNum; i++) {
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
