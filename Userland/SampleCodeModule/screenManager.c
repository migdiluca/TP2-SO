#include <syscall.h>
#include <font.h>
#include <stdint.h>
#include <screenManager.h>


void writeChar(char c, uint64_t x, uint64_t y, struct RGB color, struct RGB background, uint64_t size){
	if(c < 32 || c > 255)
		return;

	 char * posOfChar = getCharPos(c);
	for(int j = 0; j < CHAR_HEIGHT ; j++) {
		for(int i = 0, k = 128; i < CHAR_WIDTH; i++, k/=2){
			if(k & posOfChar[j])
				_syscall(_writeBlock,i*size + x, j*size + y, color,size,size);
			else
				_syscall(_writeBlock,i*size + x, j*size + y, background,size,size);
		}
	}
}

void writeString(char* string, uint64_t x, uint64_t y, struct RGB color, struct RGB background, uint64_t size){
	while(*string != 0){
		writeChar(*string,x,y,color,background,size);
		x += (CHAR_WIDTH + 1) * size;
		string++;
	}
}

void clearScreen() {
	struct RGB black={0,0,0};
	_syscall(_fillScreen,black);
}
