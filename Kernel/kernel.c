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

// void kill_rec(){
// 	uint64_t i=0, x;
// 	for(; i<100000000;i++){
// 			i--;
// 			i++;
// 			x = i;
	
// 	}
// 	kill_process(2);
// 	while(1);
	
// }

// void rec(){
// 	color c = {0,0,0};
// 	int x, y =0;
// 	char buff[] = "Hola manola\n";
// 	while(1){

// 		vdriver_text_write ( 0, buff, 13 );
// 		for(int i=0; i<10000000;i++){
// 			i--;
// 			i++;
// 			x = i;
// 		}
// 	}
// }

void idle_process(){
    while(1){
        _hlt();
    }
}



//@TODO implementar wrapper
//@TODO implementar pasaje de parametros
//@TODO implementar exit (para implementar wrapper)
//@TODO prioridad
//@TODO hacer que los stacks no usen malloc (arreglo de stacks)
//@TODO hacer que la lista del scheduler no use malloc si queremos
//@TODO el arreglo del PCB va ahi?
//@TODO poner el PCB.h en PROCESS.h (preguntar a fede si me acuerdo)
//cosas que estan en en el tp pdf

//#include <video.h>
int main()
{
	load_idt();
	my_mm_init ( heap, HEAP_SIZE );
	
	
	initialize_scheduler(new_process((uint64_t) idle_process, LOW));
	//( ( EntryPoint ) shellCodeModuleAddress ) ();
	new_process((uint64_t) shellCodeModuleAddress, HIGH);
	// int64_t pid = new_process((uint64_t) rec, HIGH);
	// new_process((uint64_t) kill_rec, HIGH);
	
	__asm__("int $0x20");
	return 0;
}
