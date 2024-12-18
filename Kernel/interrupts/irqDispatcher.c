// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <irq_dispatcher.h>



static void int_20();
static void int_21();

static void ( *irq_routines[2] ) () = {
	int_20,
	int_21
};

void irq_dispatcher ( uint64_t irq )
{
	irq_routines[irq]();
	return;
}


void int_20()
{
	timer_handler();
}
void int_21()
{
	keyboard_handler();
}
