
#ifndef PROCESS_H
#define PROCESS_H

#include <memory_management.h>
#include <stack.h>
#include <stdint.h>
#include <scheduler.h>
#include <PCB.h>
#include <shared_libc.h>
#include <shared_defs.h>
#include <semaphores.h>
#include <kernel_libc.h>
#include <driversInformation.h>
#include <pipe.h>
#include <keyboard.h>
#include <time.h>
#include <kernel.h>

#define PCB_AMOUNT 100




typedef int ( *main_function ) ( char ** argv, uint64_t argc );

int64_t new_process ( main_function rip, priority_t priority, uint8_t killable, char ** argv, uint64_t argc, fd_t fds[] );
PCB * get_pcb ( pid_t pid );
void list_processes();
int64_t kill_process ( pid_t pid );
int64_t kill_process_pcb ( PCB * pcb );
pid_t wait ( pid_t pid, int64_t * ret );
process_info_list * ps();
int8_t get_status ( pid_t pid );
void free_ps ( process_info_list * ps );
void close_fds ( PCB * pcb );
void ctrl_c_handler();
int64_t make_me_zombie ( int64_t retval );
#endif
