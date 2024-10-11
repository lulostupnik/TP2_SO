#include <stack.h>

// void process_wrapper(main_function code, /*int argc*/ ,char **argv) {
//     int len = string_array_len(argv);
//     int ret_value = code(len, argv);
//     kill_current_process(ret_value);
// }

#define RSI_POS 8
#define RDI_POS 9
#define RDX_POS 11

void process_wrapper(main_function rip, char **argv, uint64_t argc) {
    int ret = rip(argv, argc);
    while(1);
//	killCurrentProcess(retValue);
}

uint64_t load_stack(uint64_t rip, uint64_t rsp, char ** argv, uint64_t argc)
{
    stack * my_stack = (stack *) (rsp - sizeof(stack));
   
   // my_stack->regs = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    my_stack->regs.rdi = rip;
    my_stack->regs.rsi = argv;
    my_stack->regs.rdx = argc;

    //my_stack->rip = rip;
    my_stack->rip = &process_wrapper;
    my_stack->cs = 0x8;
    my_stack->rflags = 0x202;
    my_stack->rsp = rsp;
    my_stack->ss = 0x0;

    return (uint64_t) my_stack;
}