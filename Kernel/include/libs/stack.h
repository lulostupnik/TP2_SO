

// todo -> ver si es así o al revez o no se


#ifndef STACK_H
#define STACK_H

#include <stdint.h>

#define GPR_QTY 15 // son los registros que usamos en pushState y popState

typedef struct stack
{
    uint64_t gpr[GPR_QTY];  // inicializado en 0 o basura
    uint64_t rip;           // el indicado por el creador del proceso
    uint64_t cs;            // 0x8
    uint64_t rflags;        // 0x202
    uint64_t rsp;           // 
    uint64_t ss;            // 0x0
} stack;

uint64_t load_stack(uint64_t rip, uint64_t rsp);

#endif