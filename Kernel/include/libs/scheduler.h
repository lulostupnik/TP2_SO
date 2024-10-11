
#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_
#include <listADT.h>
#include <my_PCB.h>


void initialize_scheduler(int64_t idle_process_pid);
void ready(my_PCB * process);
uint64_t scheduler(uint64_t current_rsp);
void unschedule(my_PCB * process);

#endif