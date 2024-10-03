
#include <process.h>

#define STACK_SIZE 256

#define PCB_AMOUNT 100


PCB pcb_array[PCB_AMOUNT] = {0};

PCB * running; 

// queue ready 

// queue blocked

uint64_t find_free_pcb();
uint64_t block_process();
uint64_t ready_process();

pid_t new_process(uint64_t rip, int priority){

    uint64_t rsp = (uint64_t) my_malloc(STACK_SIZE) + STACK_SIZE; // - 1?
    // todo -> cambiar el BLOCK_SIZE en el memory manager para que entre el stack
    load_stack(rip, rsp);
    
    // buscar indice de PCB disponible en el arreglo de PCBs

}

uint64_t find_free_pcb(){
    uint64_t to_return = 0;

    while(pcb_array[to_return]->status != FREE){
        to_return++;
    }

    return to_return;
}

// IDEAS FUTURO:
// freePIDList o arreglo o lo que sea
