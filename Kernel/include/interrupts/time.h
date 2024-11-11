#ifndef _TIME_H_
#define _TIME_H_
#include <ordered_list_adt.h>
#include <process.h>
void timer_handler();
int ticks_elapsed();
int seconds_elapsed();
int64_t ticks_sleep ( int time );
void init_timer_handler();
void unsleep_kill ( PCB * pcb );

#endif
