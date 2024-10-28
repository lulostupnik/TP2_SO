#include <semaphores.h>

#define SEM_AMOUNT 32

typedef struct sem_structure{
    uint64_t value;
    uint8_t lock; 
    uint8_t not_free; // cantidad de procesos que tienen abierto al semáforo
    queue_adt queue;
} sem_structure;

sem_structure sem_array[SEM_AMOUNT] = {0};


int cmp(elem_type_ptr e1, elem_type_ptr e2)
{
	return e1 - e2;
}


int64_t my_sem_open(int64_t sem_id, int value){
    if((sem_id >= SEM_AMOUNT) || (sem_id < 0) ){
        return -1;
    }

    acquire(&sem_array[sem_id].lock);
    if(sem_array[sem_id].not_free){ 
        sem_array[sem_id].not_free++;
        release(&sem_array[sem_id].lock);
        return 1; 
    }

    // Si es el primero en abrirlo crea el semáforo

    queue_adt queue = new_queue();
    if(queue == NULL){
        release(&sem_array[sem_id].lock);
        return -1;
    }

    sem_array[sem_id].not_free = 1;
    sem_array[sem_id].lock = 1; 
    sem_array[sem_id].value = value;
    sem_array[sem_id].queue = queue;
    
    release(&sem_array[sem_id].lock);
    // return 2; 
    return 1;
}


int64_t my_sem_wait(int64_t sem_id){

    if((sem_id >= SEM_AMOUNT) || (sem_id < 0) ){
        return -1;
    }

    while(1){

        acquire(&sem_array[sem_id].lock);

        if(!sem_array[sem_id].not_free){
            release(&sem_array[sem_id].lock);
            return -1;
        }

        if(sem_array[sem_id].value > 0){ 
            sem_array[sem_id].value --;
            release(&sem_array[sem_id].lock);
            return 1;
        }

        PCB * running_pcb = get_running();
        block_current_no_yield();
        enqueue(sem_array[sem_id].queue, running_pcb);
        release(&sem_array[sem_id].lock); 

        scheduler_yield(); 
    }
}

int64_t my_sem_post(int64_t sem_id) {

    if((sem_id >= SEM_AMOUNT) || (sem_id < 0) ){
        return -1;
    }

    acquire(&sem_array[sem_id].lock);

    if(!sem_array[sem_id].not_free){
        release(&sem_array[sem_id].lock);
        return -1;
    }
    
    sem_array[sem_id].value++;

    if(!queue_is_empty(sem_array[sem_id].queue)){
        PCB * to_unblock = dequeue(sem_array[sem_id].queue);           
        ready(to_unblock);
    }

    release(&sem_array[sem_id].lock);
    scheduler_yield();
    return 1;
}

// todo -> chequear está función !!!!
int64_t my_sem_close(int64_t sem_id){

    if((sem_id >= SEM_AMOUNT) || (sem_id < 0) ){
        return -1;
    }

    acquire(&sem_array[sem_id].lock);
    
    if(sem_array[sem_id].not_free == 0){
        release(&sem_array[sem_id].lock);
        return -1;
    }

    sem_array[sem_id].not_free --;
    if(sem_array[sem_id].not_free == 0){
        while(!queue_is_empty(sem_array[sem_id].queue)){
            PCB * pcb = dequeue(sem_array[sem_id].queue);
            ready(pcb);
        }
        free_queue(sem_array[sem_id].queue);
        // release(&sem_array[sem_id].lock);
        // return 2;
    }
    
    release(&sem_array[sem_id].lock);
    return 1;
}
