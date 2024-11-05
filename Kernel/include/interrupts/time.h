#ifndef _TIME_H_
#define _TIME_H_
#include <ordered_list_ADT.h>
void timer_handler();
int ticks_elapsed();
int seconds_elapsed();
void nano_sleep ( int time );
void init_timer_handler();
#endif
