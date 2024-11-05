
//  Si la shell se bloquea y hay otro proceso en foreground --> este proceso tiene el pipe abierto con el buffer del teclado. 
//  Cuando se desbloquea la shell --> este tiene el pipe con el buffer del teclado. 
//  SHELL / FOREGROUND un pipe especial que STDIN es KEYBOARD y STDOUT/STDERR es VIDEO
//Array de pipe_adt[ ( NUMPROCESS/2 ) + 1 ] o similar

#include <pipe.h>



#define BAD_ID(id) (((id) < 0) || ((id) >= AMOUNT_OF_PIPES))


//Todo agregar get_available en semaphore

typedef struct pipe_cdt{
    pid_t pids[2];
    uint8_t initialized_qtty;
    uint16_t buffer[PIPE_BUFFER_SIZE];  //@TODO change vdriver_text_write para que is hay algo mayor a 255 no lo escriba. 
    uint32_t current_read; 
    uint32_t current_write;             // ¿o meter un único current?
    uint64_t data_available_sem; 	    //Todo cambiar los semaforos a sem_t
	
    // uint64_t data_available_mutex; 	   opcion 2.
	// uint8_t data_available;
    // uint64_t blocker_sem;
    
    uint64_t can_write_sem; 
    //sem_t mutex;			            //cambiar semaphore.c para que los primeros AMOUNT_OF_PIPES * 2 semaforos ID son del PIPE ADT.
} pipe_cdt;

static pipe_cdt pipes_array[AMOUNT_OF_PIPES];



void pipe_init(){
	for(int i=0; i<AMOUNT_OF_PIPES ; i++){
		my_sem_open(SEM_AMOUNT-2*i-1, 0);  //@todo que hago si falla xd?   	//Los ultimos i semaforos
        my_sem_open(SEM_AMOUNT-2*i-2, 0);  //check range. 
		pipes_array[i].data_available_sem = SEM_AMOUNT-2*i-1;
        pipes_array[i].can_write_sem = SEM_AMOUNT-2*i-2;
        pipes_array[i].pids[WRITER] = pipes_array[i].pids[READER] = -1; 
	}
}

int64_t pipe_open(int64_t id, pipe_mode_t mode){
    if( ! ((mode == READER) || (mode == WRITER)) || BAD_ID(id) || (pipes_array[id].pids[mode] != -1)){
        return -1;
    }
    pipes_array[id].pids[mode] = get_pid();
    pipes_array[id].initialized_qtty++;
    return 0;
}




int64_t pipe_get_free(){
    for(int i=0; i<AMOUNT_OF_PIPES ; i++){
        if(pipes_array[i].pids[READER] == -1 && pipes_array[i].pids[WRITER] == -1){
            return i;
        }
    }
    return -1;
}

int64_t pipe_read(int64_t id, uint16_t * buffer, uint64_t amount){
	if( BAD_ID(id) || pipes_array[id].pids[READER] != get_pid()){
        return -1;
    }
    uint64_t i = 0;
    if(my_sem_wait(pipes_array[id].data_available_sem) == -1){
        return -1;
    }
    //mutex wait ?
    int max_write = pipes_array[id].current_write;

    // if(max_write > PIPE_BUFFER_SIZE){  //@todo borrar, es para debuggear
    //     return -2;
    // }

    while ( i < amount && (pipes_array[id].current_read < max_write) && pipes_array[id].buffer[pipes_array[id].current_read] != EOF  ) { // race condition?? @todo test.....
		buffer[i++] = pipes_array[id].buffer[pipes_array[id].current_read++];
        //pipes_array[id].current_read = (pipes_array[id].current_read + 1) % PIPE_BUFFER_SIZE;
	}

    
    if(pipes_array[id].current_read < max_write){
        //my_sem_post(pipes_array[id].data_available_sem);
        my_sem_set_value(pipes_array[id].data_available_sem, 1);
    }
    if(pipes_array[id].current_read == PIPE_BUFFER_SIZE ){
        my_sem_post(pipes_array[id].can_write_sem);
        //my_sem_wait(pipes_array[id].data_available_sem);
    }
	return i;
}

//if not buffer_has_next block. 
	
int64_t pipe_write(int64_t id, uint16_t * buffer, uint64_t amount){
    if( BAD_ID(id) || pipes_array[id].pids[WRITER] != get_pid()){
        return -1;
    }
    // if(my_sem_wait(pipes_array[id].can_write_sem) == -1){
    //     return -1;
    // }
    int i=0;
    for(; i<amount ; i++){
        pipes_array[id].buffer[pipes_array[id].current_write ++] = buffer[i];
        if( pipes_array[id].current_write == PIPE_BUFFER_SIZE ){ //checkear caso limite. 
            my_sem_set_value(pipes_array[id].data_available_sem, 1);
            my_sem_wait(pipes_array[id].can_write_sem); //agregar checkeo
            pipes_array[id].current_write = 0;
            pipes_array[id].current_read = 0;
        }
    }

    my_sem_set_value(pipes_array[id].data_available_sem, 1);
    return i;
}


int64_t pipe_close(int64_t id){
    if( BAD_ID(id) ){
        return -1;
    }
    int flag = -1;
    pid_t pid = get_pid();
    if(pipes_array[id].pids[WRITER] == pid){
        uint16_t end_of_file[] = {EOF};
        pipe_write(id, end_of_file , 1 );           // @todo y si me da -1?????? lcdm
        flag = 0;
        pipes_array[id].pids[WRITER] = -1;
    }
    if(pipes_array[id].pids[READER] == pid){
        pipes_array[id].pids[READER] = -1;
        flag = 0;
    }
    if(pipes_array[id].initialized_qtty == 2 && !flag && pipes_array[id].pids[WRITER] == -1 &&  pipes_array[id].pids[READER] == -1){
        pipes_array[id].current_read = pipes_array[id].current_write = 0;
        my_sem_set_value(pipes_array[id].data_available_sem, 0);
        my_sem_set_value(pipes_array[id].can_write_sem, 1);
    }
    return flag;
}


//Hacemos un write: si la salida es pantalla, escribe en pantalla. Si la salida es un proceso lo escribimos en el buffer. 
//Cambio la syscall read para que si FD es STDIN llama a keyboard driver y si no es STDIN llama al ADT. 

//agregar sem_close y sacar sem_open de sem_init



//  Si la shell se bloquea y hay otro proceso en foreground --> este proceso tiene el pipe abierto con el buffer del teclado. 
//  Cuando se desbloquea la shell --> este tiene el pipe con el buffer del teclado. 
//  SHELL / FOREGROUND un pipe especial que STDIN es KEYBOARD y STDOUT/STDERR es VIDEO
//Array de pipe_adt[ ( NUMPROCESS/2 ) + 1 ] o similar
/*
#include <pipe.h>



#define BAD_ID(id) (((id) < 0) || ((id) >= AMOUNT_OF_PIPES))


//Todo agregar get_available en semaphore

typedef struct pipe_cdt{
    pid_t pids[2];
    uint8_t initialized_qtty;
    uint16_t buffer[PIPE_BUFFER_SIZE];  //@TODO change vdriver_text_write para que is hay algo mayor a 255 no lo escriba. 
    uint32_t current_read; 
    uint32_t current_write;             // ¿o meter un único current?
    uint64_t data_available_block; 	    //Todo cambiar los semaforos a sem_t
	
    uint8_t data_available_mutex;
	uint8_t data_available;
   
    
    uint64_t can_write_mutex;
    uint64_t can_write_block; 
    uint8_t can_write_val;
    //sem_t mutex;			            //cambiar semaphore.c para que los primeros AMOUNT_OF_PIPES * 2 semaforos ID son del PIPE ADT.
} pipe_cdt;

static pipe_cdt pipes_array[AMOUNT_OF_PIPES];
static void set_data_available(uint64_t id, uint8_t value);

static void set_data_available(uint64_t id, uint8_t value){
    if(BAD_ID(id) || (value != 1 && value != 0)){
        return;
    }
   
    aquire(&pipes_array[id].data_available_mutex);
    int last_value = pipes_array[id].data_available;
    pipes_array[id].data_available = value;
    release(&pipes_array[id].data_available_mutex);
    if(value == 1 && last_value == 0){
        my_sem_post(data_available_block);
    }
}



void pipe_init(){
	for(int i=0; i<AMOUNT_OF_PIPES ; i++){
		my_sem_open(SEM_AMOUNT-2*i-1, 0);  //@todo que hago si falla xd?   	//Los ultimos i semaforos
        my_sem_open(SEM_AMOUNT-2*i-2, 0);  //check range. 
		pipes_array[i].data_available_block = SEM_AMOUNT-2*i-1;
        pipes_array[i].can_write_sem = SEM_AMOUNT-2*i-2;
        pipes_array[i].pids[WRITER] = pipes_array[i].pids[READER] = -1; 
	}
}

int64_t pipe_open(int64_t id, pipe_mode_t mode){
    if( ! ((mode == READER) || (mode == WRITER)) || BAD_ID(id) || (pipes_array[id].pids[mode] != -1)){
        return -1;
    }
    pipes_array[id].pids[mode] = get_pid();
    pipes_array[id].initialized_qtty++;
    return 0;
}




int64_t pipe_get_free(){
    for(int i=0; i<AMOUNT_OF_PIPES ; i++){
        if(pipes_array[i].pids[READER] == -1 && pipes_array[i].pids[WRITER] == -1){
            return i;
        }
    }
    return -1;
}

int64_t pipe_read(int64_t id, uint16_t * buffer, uint64_t amount){
	if( BAD_ID(id) || pipes_array[id].pids[READER] != get_pid()){
        return -1;
    }
    uint64_t i = 0;
    if(my_sem_wait(pipes_array[id].data_available_sem) == -1){
        return -1;
    }
    //mutex wait ?
    int max_write = pipes_array[id].current_write;

    // if(max_write > PIPE_BUFFER_SIZE){  //@todo borrar, es para debuggear
    //     return -2;
    // }

    while ( i < amount && (pipes_array[id].current_read < max_write) && pipes_array[id].buffer[pipes_array[id].current_read] != EOF  ) { // race condition?? @todo test.....
		buffer[i++] = pipes_array[id].buffer[pipes_array[id].current_read++];
        //pipes_array[id].current_read = (pipes_array[id].current_read + 1) % PIPE_BUFFER_SIZE;
	}

    
    if(pipes_array[id].current_read < max_write){
        //my_sem_post(pipes_array[id].data_available_sem);
        my_sem_set_value(pipes_array[id].data_available_sem, 1);
    }
    if(pipes_array[id].current_read == PIPE_BUFFER_SIZE ){
        my_sem_post(pipes_array[id].can_write_sem);
        //my_sem_wait(pipes_array[id].data_available_sem);
    }
	return i;
}

//if not buffer_has_next block. 
	
int64_t pipe_write(int64_t id, uint16_t * buffer, uint64_t amount){
    if( BAD_ID(id) || pipes_array[id].pids[WRITER] != get_pid()){
        return -1;
    }
    // if(my_sem_wait(pipes_array[id].can_write_sem) == -1){
    //     return -1;
    // }
    int i=0;
    for(; i<amount ; i++){
        pipes_array[id].buffer[pipes_array[id].current_write ++] = buffer[i];
        if( pipes_array[id].current_write == PIPE_BUFFER_SIZE ){ //checkear caso limite. 
            my_sem_set_value(pipes_array[id].data_available_sem, 1);
            my_sem_wait(pipes_array[id].can_write_sem); //agregar checkeo
            pipes_array[id].current_write = 0;
            pipes_array[id].current_read = 0;
        }
    }

    my_sem_set_value(pipes_array[id].data_available_sem, 1);
    return i;
}


int64_t pipe_close(int64_t id){
    if( BAD_ID(id) ){
        return -1;
    }
    int flag = -1;
    pid_t pid = get_pid();
    if(pipes_array[id].pids[WRITER] == pid){
        uint16_t end_of_file[] = {EOF};
        pipe_write(id, end_of_file , 1 );           // @todo y si me da -1?????? lcdm
        flag = 0;
        pipes_array[id].pids[WRITER] = -1;
    }
    if(pipes_array[id].pids[READER] == pid){
        pipes_array[id].pids[READER] = -1;
        flag = 0;
    }
    if(pipes_array[id].initialized_qtty == 2 && !flag && pipes_array[id].pids[WRITER] == -1 &&  pipes_array[id].pids[READER] == -1){
        pipes_array[id].current_read = pipes_array[id].current_write = 0;
        my_sem_set_value(pipes_array[id].data_available_sem, 0);
        my_sem_set_value(pipes_array[id].can_write_sem, 1);
    }
    return flag;
}
*/