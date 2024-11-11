#ifndef _PCB_H
#define _PCB_H

#include <shared_defs.h>
#include <drivers_information.h>


typedef struct PCB {
	pid_t pid;
	uint64_t rsp;
	uint64_t lowest_stack_address;
	uint8_t status;
	char ** argv;
	uint64_t argc;
	int64_t ret;
	priority_t priority;
	uint8_t killable;
	struct PCB * waiting_me;
	struct PCB * waiting_for;
	int64_t blocked_by_sem;
	fd_t fds[CANT_FDS];
	uint64_t time;
	uint64_t start;
} PCB;


#endif