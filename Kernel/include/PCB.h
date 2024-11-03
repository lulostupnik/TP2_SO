#ifndef _PCB_H
#define _PCB_H

#include <shared_defs.h>



typedef struct PCB {
	uint8_t is_background; // puede que después nos convenga usar los file descriptors -> luego sabemos si es background fijandonos si si file_descriptors[stdin] == stdin (fd de la entrada de la shell)
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
//	pipeADT fds[3];
} PCB;


#endif