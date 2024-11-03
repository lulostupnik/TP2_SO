//  Si la shell se bloquea y hay otro proceso en foreground --> este proceso tiene el pipe abierto con el buffer del teclado. 
//  Cuando se desbloquea la shell --> este tiene el pipe con el buffer del teclado. 
//  SHELL / FOREGROUND un pipe especial que STDIN es KEYBOARD y STDOUT/STDERR es VIDEO
//Array de pipe_adt[ ( NUMPROCESS/2 ) + 1 ] o similar

#include <pipeADT.h>

#define PIPE_BUFFER_SIZE 508
#define AMOUNT_OF_PIPES 100

//Todo agregar get_available en semaphore

typedef struct pipe_cdt{
    uint8_t is_occupied;
    uint16_t buffer[PIPE_BUFFER_SIZE]; //@TODO change vdriver_text_write para que is hay algo mayor a 255 no lo escriba. 
    uint32_t current_read; 
    uint32_t current_write; // ¿o meter un único current?
    uint64_t data_available_sem; 	//Todo cambiar los semaforos a sem_t
	//sem_t mutex;			//cambiar semaphore.c para que los primeros AMOUNT_OF_PIPES * 2 semaforos ID son del PIPE ADT.
} pipe_cdt;

static pipe_cdt pipes_array[AMOUNT_OF_PIPES];

void pipe_init(){
	for(int i=0; i<AMOUNT_OF_PIPES ; i++){
		my_sem_open(SEM_AMOUNT-i-1, 0); 		//Los ultimos i semaforos
		pipes_array[i].data_available_sem = SEM_AMOUNT-1-i;
	}
}


//Hacemos un write: si la salida es pantalla, escribe en pantalla. Si la salida es un proceso lo escribimos en el buffer. 
//Cambio la syscall read para que si FD es STDIN llama a keyboard driver y si no es STDIN llama al ADT. 