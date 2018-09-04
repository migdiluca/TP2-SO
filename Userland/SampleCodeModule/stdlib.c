#include <stdint.h>
#include <syscall.h>

int strcmp(char * str1, char* str2) {
	int i;
	for(i = 0; str1[i] != 0 || str2[i] != 0; i++){
		if(str1[i] != str2[i])
			return 0;
	}
	return 1;
}

char * intToChar(int value,int base,char * buffer) {
	char *p = buffer;
	char *p1, *p2;
	char* digits = 0;

	//Calculate characters for each digit
	do
	{
		int remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	}
	while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}

void clockString(char * clockTime){
		int secondsInt = _syscall(_readTime,0);
        int minutesInt =_syscall(_readTime,1);
        int hourInt =_syscall(_readTime,2);

        intToChar(secondsInt,16,clockTime + 6);
        intToChar(minutesInt,16,clockTime + 3);
        intToChar(hourInt,16,clockTime);

        if(hourInt < 10) {
           clockTime[1] = clockTime[0];
           clockTime[0] = '0';
        }
        if(minutesInt < 10){
            clockTime[4] = clockTime[3];
            clockTime[3] = '0';
        }
        if(secondsInt < 10) {
            clockTime[7] = clockTime[6];
            clockTime[6] = '0';
        }

        clockTime[2] = ':';
        clockTime[5] = ':';
		clockTime[8] = '\0';
}
