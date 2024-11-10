
#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_
#include <listADT.h>
#include <PCB.h>
#include <interrupts.h>

void initialize_scheduler(pid_t shell_process_pid, pid_t idle_process_pid);
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
void unblock_waiting_me();
void unblock_waiting_pid(pid_t pid);
void unblock_waiting_pcb(PCB * pcb);
void block_current_no_yield();
PCB * get_idle_pcb();
PCB * get_shell_pcb();
void set_running_null();

#endif