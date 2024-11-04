//  Si la shell se bloquea y hay otro proceso en foreground --> este proceso tiene el pipe abierto con el buffer del teclado. 
//  Cuando se desbloquea la shell --> este tiene el pipe con el buffer del teclado. 
//  SHELL / FOREGROUND un pipe especial que STDIN es KEYBOARD y STDOUT/STDERR es VIDEO
//Array de pipe_adt[ ( NUMPROCESS/2 ) + 1 ] o similar

#include <pipeADT.h>

#define PIPE_BUFFER_SIZE 508
#define AMOUNT_OF_PIPES 100
#define EOF 0

#define BAD_ID(id) (((id) < 0) || ((id) >= AMOUNT_OF_PIPES))

typedef enum  {READER=0, WRITER} pipe_mode_t;

//Todo agregar get_available en semaphore

typedef struct pipe_cdt{
    pid_t pids[2];
    uint16_t buffer[PIPE_BUFFER_SIZE];  //@TODO change vdriver_text_write para que is hay algo mayor a 255 no lo escriba. 
    uint32_t current_read; 
    uint32_t current_write;             // ¿o meter un único current?
    uint64_t data_available_sem; 	    //Todo cambiar los semaforos a sem_t
	//sem_t mutex;			            //cambiar semaphore.c para que los primeros AMOUNT_OF_PIPES * 2 semaforos ID son del PIPE ADT.
} pipe_cdt;

static pipe_cdt pipes_array[AMOUNT_OF_PIPES];



void pipe_init(){
	for(int i=0; i<AMOUNT_OF_PIPES ; i++){
		my_sem_open(SEM_AMOUNT-i-1, 0);  //@todo que hago si falla xd?   	//Los ultimos i semaforos
		pipes_array[i].data_available_sem = SEM_AMOUNT-1-i;
        pipes_array[i].pids[WRITER] = pipes_array[i].pids[READER] = -1; 
	}
}

int64_t pipe_open(int64_t id, pipe_mode_t mode){
    if( ! ((mode == READER) || (mode == WRITER)) || BAD_ID(id) || (pipes_array[id].pids[mode] != -1)){
        return -1;
    }
    pipes_array[id].pids[mode] = get_pid();
    return 0;
}




// int64_t pipe_get_free(){

// }

int64_t pipe_read(int64_t id, uint16_t * buffer, uint64_t amount){
	if( BAD_ID(id) ){
        return -1;
    }
    uint64_t i = 0;
    if(my_sem_wait(pipes_array[id].data_available_sem) == -1){
        return -1;
    }

    //mutex down ?
    int max_write = pipes_array[id].current_write;
    //mutex up ?
    while ( i < amount && (pipes_array[id].current_read != max_write)) { // race condition?? @todo test.....
		buffer[i++] = pipes_array[id].buffer[pipes_array[id].current_read];
        pipes_array[id].current_read = (pipes_array[id].current_read + 1) % PIPE_BUFFER_SIZE;
	}
    
    if(pipes_array[id].current_read != max_write){
        //my_sem_post(pipes_array[id].data_available_sem);
        my_sem_set_value(pipes_array[id].data_available_sem, 1);
    }
	return i;
}

//if not buffer_has_next block. 
	
int64_t pipe_write(int64_t id, uint16_t * buffer, uint64_t amount){
    
}


int64_t pipe_close(int64_t id){
    if( (id < 0) || (id >= AMOUNT_OF_PIPES) ){
        return -1;
    }
    int flag = -1;
    pid_t pid = get_pid();
    if(pipes_array[id].pids[WRITER] == pid){
        pipes_array[id].pids[WRITER] = -1;
        uint16_t end_of_file[] = {EOF};
        pipe_write(id, end_of_file , 1 );           // @todo y si me da -1?????? lcdm
        flag = 0;
    }
    if(pipes_array[id].pids[READER] == pid){
        pipes_array[id].pids[READER] = -1;
        flag = 0;
    }
    if(!flag && pipes_array[id].pids[WRITER] == -1 &&  pipes_array[id].pids[READER] == -1){
        pipes_array[id].current_read = pipes_array[id].current_write = 0;
        my_sem_set_value(pipes_array[id].data_available_sem, 0);
    }
    return flag;
}


//Hacemos un write: si la salida es pantalla, escribe en pantalla. Si la salida es un proceso lo escribimos en el buffer. 
//Cambio la syscall read para que si FD es STDIN llama a keyboard driver y si no es STDIN llama al ADT. 