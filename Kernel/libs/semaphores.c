#include <semaphores.h>



typedef struct sem_structure{
    uint64_t value;
    uint8_t lock; 
    uint8_t qtty_open; 
    queue_adt queue;
} sem_structure;

sem_structure sem_array[SEM_AMOUNT + KERNEL_SEM_AMOUNT] = {0};

static int8_t is_valid_id(int64_t sem_id, uint8_t is_kernel);
static int is_value_zero(sem_structure *sem);
static int always_true(sem_structure *sem);
static int64_t post_if_condition(int64_t sem_id, int (*condition)(sem_structure *), uint8_t is_kernel);

int cmp(elem_type_ptr e1, elem_type_ptr e2)
{
	return e1 - e2;
}

int64_t my_sem_post(int64_t sem_id, uint8_t is_kernel) {
    return post_if_condition(sem_id, always_true, is_kernel);
}                

int64_t sem_post_if_value_is_zero(int64_t sem_id, uint8_t is_kernel){
   return post_if_condition(sem_id, is_value_zero, is_kernel);
}

int64_t my_sem_open(int64_t sem_id, int value, uint8_t is_kernel){
    if(!is_valid_id(sem_id, is_kernel)){
        return -1;
    }

    acquire(&sem_array[sem_id].lock);
    if(sem_array[sem_id].qtty_open){ 
        sem_array[sem_id].qtty_open++;
        release(&sem_array[sem_id].lock);
        return 0; 
    }

    // Si es el primero en abrirlo crea el semáforo

    queue_adt queue = new_queue();
    if(queue == NULL){
        release(&sem_array[sem_id].lock);
        return -1;
    }

    sem_array[sem_id].qtty_open = 1;
    sem_array[sem_id].lock = 1; 
    sem_array[sem_id].value = value;
    sem_array[sem_id].queue = queue;
    
    release(&sem_array[sem_id].lock);
    return 0;
}


int64_t my_sem_wait(int64_t sem_id, uint8_t is_kernel){

    if(!is_valid_id(sem_id, is_kernel)){
        return -1;
    }

        acquire(&sem_array[sem_id].lock);

        if(!sem_array[sem_id].qtty_open){
            release(&sem_array[sem_id].lock);
            return -1;
        }

        if(sem_array[sem_id].value > 0){ 
            sem_array[sem_id].value --;
            release(&sem_array[sem_id].lock);
            return 0;                           
        }

        PCB * running_pcb = get_running();
        block_current_no_yield();
        running_pcb->blocked_by_sem = sem_id;
        enqueue(sem_array[sem_id].queue, running_pcb);
        release(&sem_array[sem_id].lock); 
        scheduler_yield(); 
        running_pcb->blocked_by_sem = -1;
}


int64_t my_sem_close(int64_t sem_id, uint8_t is_kernel){

    if(!is_valid_id(sem_id, is_kernel)){
        return -1;
    }

    acquire(&sem_array[sem_id].lock);
    
    if(sem_array[sem_id].qtty_open == 0){
        release(&sem_array[sem_id].lock);
        return -1;
    }

    sem_array[sem_id].qtty_open --;
    if(sem_array[sem_id].qtty_open == 0){
        while(!queue_is_empty(sem_array[sem_id].queue)){
            PCB * pcb = dequeue(sem_array[sem_id].queue);
            ready(pcb);
        }
        free_queue(sem_array[sem_id].queue);
    }
    
    release(&sem_array[sem_id].lock);
    return 0;                                                             
}


int64_t delete_from_blocked_queue(PCB * pcb){
    
    if( pcb == NULL){
        return -1;
    }
    if(pcb->blocked_by_sem != -1){
        delete_from_queue(sem_array[pcb->blocked_by_sem ].queue, pcb);
	}
    return 0;
}

static int64_t post_if_condition(int64_t sem_id, int (*condition)(sem_structure *), uint8_t is_kernel) {
    if (!is_valid_id(sem_id, is_kernel)) {
        return -1;
    }

    acquire(&sem_array[sem_id].lock);

    if (!sem_array[sem_id].qtty_open) {
        release(&sem_array[sem_id].lock);
        return -1;
    }

    if (condition(&sem_array[sem_id])) {
        if (queue_is_empty(sem_array[sem_id].queue)) {
             sem_array[sem_id].value++;
        } else {
            PCB *to_unblock = dequeue(sem_array[sem_id].queue);
            ready(to_unblock);
        }
    }

    release(&sem_array[sem_id].lock);
    scheduler_yield();
    return 0;
}

static int is_value_zero(sem_structure *sem) {
    return sem->value == 0;
}

static int always_true(sem_structure *sem) {
    return 1; 
}

static int8_t is_valid_id(int64_t sem_id, uint8_t is_kernel){
    if(is_kernel){
        return (sem_id >= SEM_AMOUNT) && (sem_id < KERNEL_SEM_AMOUNT);
    }
    return !((sem_id >= SEM_AMOUNT) || (sem_id < 0)) ;
}
