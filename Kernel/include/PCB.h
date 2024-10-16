#ifndef _PCB_H
#define _PCB_H



typedef enum { LOW = 0, MEDIUM, HIGH } priority_t;

typedef struct PCB {
	int64_t pid, ppid;
	uint64_t rsp;
	uint64_t base_pointer;
	uint8_t status;
	char ** argv;
	uint64_t argc;
	int64_t ret;
	priority_t priority;
	uint8_t killable;
} PCB;


#endif