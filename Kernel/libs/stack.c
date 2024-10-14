#include <stack.h>



void process_wrapper(main_function rip, char **argv, uint64_t argc, uint64_t pid) {
    int ret = rip(argv, argc);
    PCB * pcb= get_pcb(pid);
    if(pcb == NULL){
        return;
    }
    pcb->ret = ret;  //@TODO esto podria estar en kill_process
    kill_process(pid);
    timer_tick();
   // __asm__("int $0x20"); //@todo cambiar 
//	killCurrentProcess(retValue);
}

uint64_t load_stack(uint64_t rip, uint64_t rsp, char ** argv, uint64_t argc, uint64_t pid)
{
    stack * my_stack = (stack *) (rsp - sizeof(stack));
   
    my_stack->regs.rdi = rip;
    my_stack->regs.rsi = argv;
    my_stack->regs.rdx = argc;
    my_stack->regs.rcx = pid;
    //my_stack->rip = rip;
    my_stack->rip = &process_wrapper;
    my_stack->cs = 0x8;
    my_stack->rflags = 0x202;
    my_stack->rsp = rsp;
    my_stack->ss = 0x0;

    return (uint64_t) my_stack;
}