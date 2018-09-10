#include <time.h>

typedef struct{
	int ticks;
	void (*callback)();
}timer;

static unsigned long ticks = 0;

timer timers[30];
int index = 0;

void checkTimers();

void timer_handler() {
	ticks++;
	checkTimers();
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 18;
}



void newTimer(int ticks, void (*callback)()){
	timers[index].ticks = ticks;
	timers[index].callback = callback;
	index++;
}

void checkTimers(){
	for(int i=0; i < index; i++){
		timers[i].ticks--;
		if(timers[i].ticks == 0){
			timers[i].callback();
			for(int j=i; j < index; j++){
				timers[j]=timers[j+1];
				index--;
			}
		}
	}
}
