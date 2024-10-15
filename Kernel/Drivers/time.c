// this is a personal academic project. dear pvs-studio, please check it.
// pvs-studio static code analyzer for c, c++ and c#: http://www.viva64.com

#include <time.h>
extern void _hlt();
static unsigned long ticks = 0;

void timer_handler()
{
	ticks++;
}

int ticks_elapsed()
{
	return ticks;
}

int seconds_elapsed()
{
	return ticks / 18;
}

void nano_sleep ( int time )
{
	int start = ticks;
	while ( ticks - start < time ) {
		_hlt();
	}
}