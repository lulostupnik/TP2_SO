#include <semaphores.h>

#define SEM_AMOUNT 32

typedef struct sem_structure{
    uint64_t value;
    uint8_t lock; 
    uint8_t not_free;
    queue_adt queue;
    ordered_list_adt process_list;
    
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
        int to_return = add_ordered_list(sem_array[sem_id].process_list, get_running());
        release(&sem_array[sem_id].lock);
        return to_return;
    }
    // Si es el primero en abrirlo crea el semáforo

    queue_adt queue = new_queue();
    if(queue == NULL){
        release(&sem_array[sem_id].lock);
        return -1;
    }
    ordered_list_adt list = new_ordered_list(cmp); // TODO -> CMP!!!!
    if(list == NULL){
        free_queue(queue);
        release(&sem_array[sem_id].lock);
        return -1;
    }
    if(add_ordered_list(list, get_running()) == -1){
        free_queue(queue);
        free_ordered_list(list);
        release(&sem_array[sem_id].lock);
        return -1;
    }


    sem_array[sem_id].not_free = 1;
    sem_array[sem_id].lock = 1; // todo -> invertir para acquire / release
    sem_array[sem_id].value = value;
    sem_array[sem_id].queue = queue;
    sem_array[sem_id].process_list = list;
    
    release(&sem_array[sem_id].lock);
    return 1; // todo -> ??
}


int64_t my_sem_wait(int64_t sem_id){
    if((sem_id >= SEM_AMOUNT) || (sem_id < 0) ){
        return -1;
    }

    
    // todo -> chequear que el semaforo esté abierto para el proceso que quiere hacer wait
    // todo -> ¿hacer acquire antes? ACQUIRE !!!
    acquire(&sem_array[sem_id].lock);
    if(!sem_array[sem_id].not_free /*|| (belongs_ordered_list(sem_array[sem_id].process_list, get_running()))*/){ // todo -> ¿está bien?
        release(&sem_array[sem_id].lock);
        return -1;
    }

    release(&sem_array[sem_id].lock);

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

    acquire(&sem_array[sem_id].lock);
    
    // if(!belongs_ordered_list(sem_array[sem_id].process_list, get_pcb(get_pid()))){
    //     return -1;
    // }
    
    sem_array[sem_id].value++;
    PCB * to_unblock ;
    if(!queue_is_empty(sem_array[sem_id].queue)){
        to_unblock = dequeue(sem_array[sem_id].queue);           
        if(to_unblock->status != BLOCKED){
            sem_id++;
            sem_id--;
        }
        ready(to_unblock);
    }

    release(&sem_array[sem_id].lock);
    PCB * p = get_pcb(get_pid());
    scheduler_yield();
    return 1;
}

// todo -> chequear está función !!!!
int64_t my_sem_close(int64_t sem_id){
    PCB * pcb;

    acquire(&sem_array[sem_id].lock);
    
    if(sem_array[sem_id].not_free == 0){
        release(&sem_array[sem_id].lock);
        return -1;
    }
    
    if(delete_ordered_list(sem_array[sem_id].process_list, get_running()) == -1){
        release(&sem_array[sem_id].lock);
        return -1;
    }
    
    if(is_empty_ordered_list(sem_array[sem_id].process_list)){
        free_queue(sem_array[sem_id].queue);
        free_ordered_list(sem_array[sem_id].process_list);
        sem_array[sem_id].not_free = 0;
    }
    
    release(&sem_array[sem_id].lock);
    return 1;
}


// sem_t get_free_sem(){
//     if(sems_used == SEM_AMOUNT){
//         return -1;
//     }
//     for(int i = 0; i < SEM_AMOUNT; i++){
//         acquire(&sem_array[i].lock);
//         if(sem_array[i].free){
//             sem_array[i].free = 0;
//             return i;
//         }
//         release(&sem_array[i].lock);
//     }
//     return -1;
// }

