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

static void * const shellCodeModuleAddress = ( void* ) 0x400000;
static void * const shellDataModuleAddress = ( void* ) 0x500000;

static void * const heap = ( void * ) 0x600000;




typedef int ( *EntryPoint ) ();


void clearBSS ( void * bssAddress, uint64_t bssSize )
{
	memset ( bssAddress, 0, bssSize );
}

void * get_stack_base()
{
	return ( void* ) (
	           ( uint64_t ) &endOfKernel
	           + PageSize * 8				//The size of the stack itself, 32KiB
	           - sizeof ( uint64_t )			//Begin at the top of the stack
	       );
}

void * initializeKernelBinary()
{
	void * moduleAddresses[] = {
		shellCodeModuleAddress,
		shellDataModuleAddress
	};
	load_modules ( &endOfKernelBinary, moduleAddresses );
	clearBSS ( &bss, &endOfKernel - &bss );
	return get_stack_base();
}


// void rec(){
// 	color c = {100,10,10};
// 	color c2 = {100,100,100};
// 	while(1){
// 		vdriver_set_mode(VIDEO_MODE, c);
//     	vdriver_video_draw_rectangle(10,10,20,20, c2);
// 		vdriver_set_mode(TEXT_MODE, c);
// 	}
// }

void idle_process(){
    while(1){
        _hlt();
		//rec();
    }
}


//#include <video.h>
int main()
{
	load_idt();
	my_mm_init ( heap, HEAP_SIZE );
	
	
	initialize_scheduler(new_process((uint64_t) idle_process, LOW));
	   
	//( ( EntryPoint ) shellCodeModuleAddress ) ();
	new_process((uint64_t) shellCodeModuleAddress, HIGH);
	//new_process((uint64_t) rec, HIGH);
	
	__asm__("int $0x20");
	return 0;
}
