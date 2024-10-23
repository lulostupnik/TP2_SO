
#ifndef PROCESS_H
#define PROCESS_H

#include <memory_management.h>
#include <stack.h>
#include <stdint.h>
#include <scheduler.h>
#include <PCB.h>
#include <shared_libc.h>

#define STACK_SIZE 0x0FFF
#define PCB_AMOUNT 100


typedef struct process_info {
	pid_t pid;
	pid_t ppid;
	priority_t priority;
	uint64_t base_pointer;
	uint64_t stack_pointer;
	uint8_t status;
} process_info;

typedef struct process_info_list {
	uint64_t amount_of_processes;
	process_info * processes;
} process_info_list;

typedef int (*main_function)(char ** argv, uint64_t argc);


int64_t new_process(main_function rip, priority_t priority, uint8_t killable, char ** argv, uint64_t argc);
PCB * get_pcb(pid_t pid);
void list_processes();
int64_t kill_process(pid_t pid);

#endif
