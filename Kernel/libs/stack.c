#include <stack.h>



uint64_t load_stack(uint64_t rip, uint64_t rsp)
{
    stack * my_stack = (stack *) (rsp - sizeof(stack));
    for(int i = 0; i < GPR_QTY; i++){
        my_stack->gpr[i] = 0;
    }
    my_stack->rip = rip;
    my_stack->cs = 0x8;
    my_stack->rflags = 0x202;
    my_stack->rsp = rsp;
    my_stack->ss = 0x0;

    return (uint64_t) my_stack;
}