
#ifndef PROCESS_H
#define PROCESS_H

#include <memory_management.h>
#include <stack.h>
#include <stdint.h>
#include <scheduler.h>
#include <PCB.h>
#include <shared_libc.h>
#include <shared_defs.h>

#define PCB_AMOUNT 100




typedef int (*main_function)(char ** argv, uint64_t argc);

// extern uint64_t amount_of_processes;


int64_t new_process(main_function rip, priority_t priority, uint8_t killable, char ** argv, uint64_t argc);
PCB * get_pcb(pid_t pid);
void list_processes();
int64_t kill_process(pid_t pid);
pid_t wait(pid_t pid, int64_t * ret);
process_info_list * ps();

#endif
