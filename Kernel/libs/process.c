
#include <process.h>

#define STACK_SIZE 256

#define PCB_AMOUNT 100



PCB pcb_array[PCB_AMOUNT] = {0}; //@todo va aca?


static int64_t find_free_pcb();



int64_t new_process(uint64_t rip, priority_t priority){

    
    // uint64_t rsp = (uint64_t) my_malloc(STACK_SIZE);
    // if(rsp == NULL){
    //     return -1;
    // }
    // rsp += STACK_SIZE;
    
    uint64_t rsp = (uint64_t) my_malloc(STACK_SIZE*4) + STACK_SIZE*4;  //@TODO No hacerlo dinamico. Definir zona de STACKS y hacer un arreglo
    
    if(rsp == STACK_SIZE*4){  //Este checkeo con la zona estatica no lo tendriamos que hacer
        return -1;
    }

    // todo -> cambiar el BLOCK_SIZE en el memory manager para que entre el stack
    rsp = load_stack(rip, rsp);
    
    int64_t pid = find_free_pcb();
    if(pid == -1){
        return -1;
    }

    
    pcb_array[pid].pid = pid;
   // pcb_array[pid].ppid = running->pid;
    pcb_array[pid].rsp = rsp;
    pcb_array[pid].status = READY;

    ready(&pcb_array[pid]);
    // ready_queue.push((void *)pcb_array + pid * sizeof(PCB));


    return pid;

}

static int64_t find_free_pcb(){
    int64_t to_return = 0;

    while(pcb_array[to_return].status != FREE && to_return < PCB_AMOUNT){
        to_return++;
    }

    if(to_return == PCB_AMOUNT){
        return -1;
    }

    return to_return;
}

PCB * get_pcb(int64_t pid){
    if(pid >= PCB_AMOUNT || pid < 0){
        return NULL;
    }
    return &pcb_array[pid];
}



int64_t kill_process(int64_t pid){
    if(pid >= PCB_AMOUNT || pid < 0 || pcb_array[pid].status == FREE){
        return -1;
    }
    unschedule(&pcb_array[pid]);
    pcb_array[pid].status = ZOMBIE;
}



// IDEAS FUTURO:
// freePIDList o arreglo o lo que sea
