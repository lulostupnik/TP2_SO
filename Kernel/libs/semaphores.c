#include <semaphores.h>

#define SEM_AMOUNT 32

typedef struct sem_structure{
    uint64_t value;
    uint8_t lock; 
    uint8_t not_free;
    queue_adt queue;
} sem_structure;

sem_structure sem_array[SEM_AMOUNT] = {0};

int64_t my_sem_open(int64_t sem_id, int value){
    if(sem_array[sem_id].not_free){
        return 2; // el semaforo ya está creado
    }

    queue_adt queue;
    if((queue = my_malloc(sizeof(*(void *)queue))) == NULL){
        return 0;
    }

    sem_array[sem_id].not_free = 1;
    sem_array[sem_id].lock = 1; // todo -> invertir para acquire / release
    sem_array[sem_id].value = value;
    sem_array[sem_id].queue = queue;

    return 1; // todo -> ??
}


int64_t my_sem_wait(int64_t sem_id){

    if(!sem_array[sem_id].not_free){ // todo -> ¿está bien?
        return -1;
    }

    while(1){

        acquire(&sem_array[sem_id].lock);

        if(!sem_array[sem_id].not_free){ // todo -> ¿está bien? -> si se libera el semáforo mientras se espera
            release(&sem_array[sem_id].lock);
            return -1;
        }

        if(sem_array[sem_id].value != 0){ // o > 0 ?
            sem_array[sem_id].value --;
            release(&sem_array[sem_id].lock);
            return 1;
        }

        PCB * running_pcb = get_running();
        running_pcb->status = BLOCKED;
        enqueue(sem_array[sem_id].queue, running_pcb);
        release(&sem_array[sem_id].lock); 

        scheduler_yield(); // recién va a volver cuando sea desbloqueado por post
        // todo -> ¿está bien esto?
    }
}

int64_t my_sem_post(int64_t sem_id) {
    acquire(&sem_array[sem_id].lock);
    sem_array[sem_id].value++;

    if(!queue_is_empty(sem_array[sem_id].queue)){
        PCB * to_unblock = dequeue(sem_array[sem_id].queue);
        to_unblock->status = READY;
    }

    release(&sem_array[sem_id].lock);
    return 1;
}

// todo -> chequear está función !!!!
int64_t my_sem_close(int64_t sem_id){
    PCB * pcb;
    acquire(&sem_array[sem_id].lock);
    //todo -> ¿retornar 0 si no existía?
    sem_array[sem_id].not_free = 0;
    queue_adt queue = sem_array[sem_id].queue;
    
    while(!queue_is_empty(queue)){ // recordar que queue_is_empty devuelve 0 si queue es NULL
        pcb = dequeue(queue);
        pcb->status = READY;
    }
    my_free(queue);

    release(&sem_array[sem_id].lock);
    return 1;
}


// traete los semafaros
/*
wait(uint64_t sem_id){
acquire(); // creo que tengo que agregarle una variable lock al semáforo
if(sem_array[sem_id].valor  != 0){
sem_array[sem_id].valor --;
release(); // pasarle lock?
return;
}


// marcar al proceso como bloqueado
// running.status = BLOCKED;
// meter el running en la lista (o lo que sea) de bloqueados por este sem
// queue(sem_array[sem_id].queue, running_pcb);

release(); // pasarle lock? -> ¿está bien está ubicación?

yield(); // recién va a volver cuando sea desbloqueado por post
return;
}
 */

/*
my_sem_t get_free_sem(){
    if(sems_used == SEM_AMOUNT){
        return -1;
    }
    for(int i = 0; i < SEM_AMOUNT; i++){
        if(sem_array[i].free){
            sem_array[i].free = 0;
            return i;
        }
    }
    return -1;
}
*/
