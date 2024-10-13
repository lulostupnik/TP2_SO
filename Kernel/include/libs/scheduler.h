
#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_
#include <listADT.h>
#include <PCB.h>


void initialize_scheduler(int64_t idle_process_pid);
void ready(PCB * process);
uint64_t scheduler(uint64_t current_rsp);
void unschedule(PCB * process);
// uint64_t yield();
uint64_t get_pid();
uint64_t block_arbitrary(uint64_t pid);
uint64_t unblock_arbitrary(uint64_t pid);
#endif