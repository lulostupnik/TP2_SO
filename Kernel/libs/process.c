
#include <process.h>

#define STACK_SIZE 256

#define PCB_AMOUNT 100



PCB pcb_array[PCB_AMOUNT] = {0}; //@todo va aca?


int64_t find_free_pcb();
uint64_t block_process();
uint64_t ready_process();
#include <video.h>  //@todo borrar

int64_t new_process(uint64_t rip, int priority){

    
    // uint64_t rsp = (uint64_t) my_malloc(STACK_SIZE);
    // if(rsp == NULL){
    //     return -1;
    // }
    // rsp += STACK_SIZE;
    
    uint64_t rsp = (uint64_t) my_malloc(STACK_SIZE) + STACK_SIZE;  //@TODO No hacerlo dinamico. Definir zona de STACKS y hacer un arreglo
    
    if(rsp == STACK_SIZE){  //Este checkeo con la zona estatica no lo tendriamos que hacer
        color c = {10,10,10};
        vdriver_video_draw_rectangle(0,0,100,100, c);
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

    ready(pcb_array[pid]);
    // ready_queue.push((void *)pcb_array + pid * sizeof(PCB));


    return pid;

}

int64_t find_free_pcb(){
    int64_t to_return = 0;

    while(pcb_array[to_return].status != FREE && to_return < PCB_AMOUNT){
        to_return++;
    }

    if(to_return == PCB_AMOUNT){
        return -1;
    }

    return to_return;
}




// IDEAS FUTURO:
// freePIDList o arreglo o lo que sea
