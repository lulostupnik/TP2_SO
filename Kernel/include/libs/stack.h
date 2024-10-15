#ifndef STACK_H
#define STACK_H

#include <stdint.h>
#include <process.h>
#include <stack_registers.h>



typedef struct stack {
	stack_registers regs;
	uint64_t rip;
	uint64_t cs;
	uint64_t rflags;
	uint64_t rsp;
	uint64_t ss;
} stack;

uint64_t load_stack(uint64_t rip, uint64_t rsp, char ** argv, uint64_t argc, uint64_t pid);

#endif