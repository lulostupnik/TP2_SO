#include <scheduler.h>

static list_adt ready_list;
static list_adt blocked_list;

static PCB * running = NULL;
static uint64_t times_ran = 0;

static PCB * idle_pcb;
static int initialized = 0;

int compare_elements(elem_type_ptr e1, elem_type_ptr e2) {
    return e1 - e2; 
}


void initialize_scheduler(int64_t idle_process_pid){
    t_compare cmp = compare_elements;
    ready_list = new_list(cmp);
    blocked_list = new_list(cmp);
    idle_pcb = get_pcb(idle_process_pid);
   // delete_list(ready_list, idle_pcb);
    initialized = 1;
    return;
}

// void finish_init(){
//     initialized = 1;
// }


void ready(PCB * process){
    process->status = READY;
    add_list(ready_list, process);
    delete_list(blocked_list, process);

}   

void block(PCB * process){
    process->status = BLOCKED;
    delete_list(ready_list, process);
    add_list(blocked_list, process);

}

void unschedule(PCB * process){
    if(process->status == READY){
        delete_list(ready_list, process);
    }else if(process->status == BLOCKED){
        delete_list(blocked_list, process);
    }
   
}
/*
uint64_t yield(){
    PCB * next_pcb = next(ready_list);
    times_ran = 0;
    if(next_pcb!=NULL){
        running = next_pcb;
        return 0;
    }
    return 1;
}
*/


void scheduler_yield(){
    if(running == NULL || is_empty_list(ready_list)){
        return;
    }
    times_ran = running->priority;
    timer_tick();
}


int64_t get_pid(){
    return running->pid;
}

uint64_t block_arbitrary(int64_t pid){
    PCB * process = get_pcb(pid);
    if (process==NULL){
        return -1;
    }
    block(process);
    return 0;
}

uint64_t unblock_arbitrary(int64_t pid){
    PCB * process = get_pcb(pid);
    if (process==NULL){
        return -1;
    }
    ready(process);
    return 0;

}

uint64_t scheduler(uint64_t current_rsp){
    if(!initialized){
        return current_rsp;
    }
    if(running != NULL){
        running->rsp = current_rsp;
    }
    if(is_empty_list(ready_list)){
        running = idle_pcb;   
        return idle_pcb->rsp; 
    }
    if(running == NULL){ // Running is null but list is not empty
        running = next(ready_list);
        return running->rsp;
    }
    if(times_ran >= running->priority){
        PCB * next_pcb = next(ready_list);
        times_ran = 0;
        running = next_pcb;
        return next_pcb->rsp;
    }
    times_ran++;
    return current_rsp;
}

int64_t nice ( int64_t pid, uint64_t new_prio ){
    PCB * process = get_pcb(pid);
    if (process==NULL){
        return -1;
    }
    process->priority = new_prio;
    return 0;
}




    /*
    if(is_empty_list(ready_list)){
        //while(1){_hlt();}
        return; 
    }
    if(running==null){
        list_adt(to_begin(ready_list));
        running = next(ready_list);
        running->status = running;
        return;
    }
    
    if(running->status == running){
        running->status = ready;
        add_list(ready_list, running);
    }
    else if(running->status == blocked){
        delete_list(ready_list, running);
        add_list(blocked_list, running);
    }

    pcb * next_process = null;

    delete_list(ready_list, next_process);
    next_process->status = running;
    context_switch(running, next_process);
    running=next_process;*/