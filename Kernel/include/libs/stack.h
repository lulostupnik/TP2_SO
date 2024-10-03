
#ifndef STACK_H
#define STACK_H

#include <stdint.h>


#define GPR_QTY 10 // todo -> cambiar

// todo -> ver si es así o al revez o no se
typedef struct stack{
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
    // align ni idea
    uint64_t gpr[GPR_QTY]; // inicializado en 0
}stack;

void load_stack(uint64_t tip, uint64_t rsp);

#endif
