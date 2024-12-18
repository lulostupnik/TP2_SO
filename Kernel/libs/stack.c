// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <stack.h>



void process_wrapper ( main_function rip, char ** argv, uint64_t argc, pid_t pid )
{
	int ret = rip ( argv, argc );
	_cli();
	PCB * pcb = get_pcb ( pid );
	if ( pcb == NULL ) {
		return;
	}
	make_me_zombie ( ret );
	timer_tick();
}

uint64_t load_stack ( uint64_t rip, uint64_t rsp, char ** argv, uint64_t argc, pid_t pid )
{
	stack * my_stack = ( stack * ) ( rsp - sizeof ( stack ) );

	my_stack->regs.rdi = rip;
	my_stack->regs.rsi = ( uint64_t ) argv;
	my_stack->regs.rdx = argc;
	my_stack->regs.rcx = pid;
	my_stack->rip = ( uint64_t ) &process_wrapper;
	my_stack->cs = 0x8;
	my_stack->rflags = 0x202;
	my_stack->rsp = rsp;
	my_stack->ss = 0x0;

	return ( uint64_t ) my_stack;
}