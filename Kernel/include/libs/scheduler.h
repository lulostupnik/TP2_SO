
#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_
#include <listADT.h>
#include <PCB.h>
#include <interrupts.h>

void initialize_scheduler(int64_t idle_process_pid);
void ready(PCB * process);
uint64_t scheduler(uint64_t current_rsp);
void unschedule(PCB * process);
// uint64_t yield();
void scheduler_yield();
int64_t get_pid();
uint64_t block_arbitrary(int64_t pid);
uint64_t unblock_arbitrary(int64_t pid);
int64_t nice ( int64_t pid, uint64_t new_prio );
#endif