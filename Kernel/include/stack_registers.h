#ifndef _STACK_REGISTERS_H
#define _STACK_REGISTERS_H

#include <stdint.h>

typedef struct {
	uint64_t r15, r14, r13, r12, r11, r10, r9, r8, rsi, rdi, rbp, rdx, rcx, rbx, rax;
} stack_registers;


#endif