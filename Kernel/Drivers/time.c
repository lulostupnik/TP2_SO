#include <time.h>
#include <lib.h>
static unsigned long ticks = 0;

void timerHandler() {
	ticks++;
}

int ticksElapsed() {
	return ticks;
}

int secondsElapsed() {
	return ticks / 18;
}

void nano_sleep(int time){
	int start = ticks;
	while(ticks - start < time){
		hlt();
	}
}