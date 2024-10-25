
#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_
#include <listADT.h>
#include <PCB.h>
#include <interrupts.h>

void initialize_scheduler(pid_t idle_process_pid);
void ready(PCB * process);
uint64_t scheduler(uint64_t current_rsp);
void unschedule(PCB * process);
void scheduler_yield();
int64_t get_pid();
uint64_t block_arbitrary(pid_t pid);
uint64_t unblock_arbitrary(pid_t pid);
int64_t nice ( pid_t pid, uint64_t new_prio );
void block_current();
PCB * get_running();
int64_t make_me_zombie(int64_t retval);
void unblock_waiting_me();
void unblock_waiting_pid(pid_t pid);
void block_current_no_yield();
#endif