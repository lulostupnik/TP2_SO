// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <kernel.h>
extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;
static const uint64_t PageSize = 0x1000;



const uint64_t shell_code_module_addr_int = 0x400000;
const uint64_t shell_data_module_addr_int = 0x500000;
const uint64_t heap_addr_int = 0x600000;

static void * const shell_code_module_address = (void *) * ( &shell_code_module_addr_int );
static void * const shell_data_module_address = (void *) * ( &shell_data_module_addr_int );
static void * const heap = (void *) * ( &heap_addr_int );


static memory_manager_adt kernel_mem, userland_mem;
typedef int ( *EntryPoint ) ();


void clearBSS ( void * bssAddress, uint64_t bssSize )
{
	shared_libc_memset ( bssAddress, 0, bssSize );
}

void * get_stack_base()
{
	return ( void * ) (
	           ( uint64_t ) &endOfKernel
	           + PageSize * 8					//The size of the stack itself, 32KiB
	           - sizeof ( uint64_t )			//Begin at the top of the stack
	       );
}

void * initializeKernelBinary()
{
	void * moduleAddresses[] = {
		shell_code_module_address,
		shell_data_module_address
	};
	load_modules ( &endOfKernelBinary, moduleAddresses );
	clearBSS ( &bss, &endOfKernel - &bss );
	return get_stack_base();
}


void idle_process()
{
	while ( 1 ) {
		_hlt();
	}
}

memory_manager_adt get_userland_mem()
{
	return userland_mem;
}
memory_manager_adt get_kernel_mem()
{
	return kernel_mem;
}


int main()
{
	load_idt();
	kernel_mem = my_mm_init ( heap );
	userland_mem = my_mm_init ( heap + HEAP_SIZE );
	char * argv_idle[] = {"idle"};
	char * argv_shell[] = {"sh"};
	fd_t idle_fds[3] = {-1, -1, -1};
	fd_t shell_fds[3] = {STDOUT, STDERR, STDIN};
	initialize_scheduler ( new_process ( ( main_function ) shell_code_module_address, HIGH, 0, argv_shell, 1, shell_fds ), new_process ( ( main_function ) idle_process, LOW, 0, argv_idle, 1, idle_fds ) );
	pipe_init();
	init_timer_handler();
	timer_tick();
	return 0;
}