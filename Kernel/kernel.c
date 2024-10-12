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

// void kill_p(){
// 	int i=0;
// 	// while(i++ < 10000){
// 	// 	int x =0;
// 	// 	for(int i=0; i<10000000;i++){
// 	// 		i--;
// 	// 		i++;
// 	// 		x = i;
// 	// 	}
// 	// 	vdriver_text_write ( 0, "waiting...\n", 17);
// 	// }
// 	vdriver_text_write ( 0, "Haha im a killer\n", 17);
// 	while(!kill_process(2)){
// 		vdriver_text_write ( 0, "Could not kill\n", 16 );
// 	}
// }

// void write_arg(char ** argv, uint64_t argc){
// 	if(argc != 1 ){
// 		return;
// 	}
// 	int x = 0;
// 	while(1){
// 		vdriver_text_write ( 0, argv[0], 13);
// 		for(int i=0; i<10000000;i++){
// 			i--;
// 			i++;
// 			x = i;
// 		}
// 	}
// }
/*
void a(){
	int x;
		while(1){
		vdriver_text_write ( 0, "A\n", 1);
		for(int i=0; i<10000000;i++){
			i--;
			i++;
			x = i;
		}
}}


void b(){
	int x;
		while(1){
		vdriver_text_write ( 0, "b\n", 1);
		for(int i=0; i<10000000;i++){
			i--;
			i++;
			x = i;
		}
}}

void c(){
	int x;
		while(1){
		vdriver_text_write ( 0, "c\n", 1);
		for(int i=0; i<10000000;i++){
			i--;
			i++;
			x = i;
		}
}}*/


void idle_process(){
    while(1){
        _hlt();
    }
}


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
	

	initialize_scheduler(new_process((main_function) idle_process, LOW, NULL, 0));
	//( ( EntryPoint ) shellCodeModuleAddress ) ();

	new_process((main_function) shellCodeModuleAddress, HIGH, NULL, 0);
	
	//new_process((main_function) a, HIGH, NULL, 0);
	//new_process((main_function) b, MEDIUM, NULL, 0);
	//new_process((main_function) c, LOW, NULL, 0);


	// char arg[] = "Dont kill me #<\n";
	// char * argv[] = {arg};
	// uint64_t pid = new_process((main_function) write_arg, HIGH, argv, 1);

	// new_process((main_function) kill_p, HIGH, NULL, 0);
	timer_tick();
	//__asm__("int $0x20");
	return 0;
}
