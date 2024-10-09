#include <scheduler.h>
uint8_t initialized = 0;

static list_adt ready_list;
static list_adt blocked_list;
static PCB * running = NULL;

int compare_elements(elem_type_ptr e1, elem_type_ptr e2) {
    return e1 - e2; 
}

void initialize_scheduler(){
    t_compare cmp = compare_elements;
    ready_list = new_list(cmp);
    blocked_list = new_list(cmp);
    initialized = 1;
    return;
}



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



uint64_t scheduler(uint64_t current_rsp){
    if(is_empty_list(ready_list) || running == NULL){
        
        //idle
        return current_rsp; // CAMBIAR
    }

    running->rsp = current_rsp;
    PCB * next_pcb = next(ready_list);
    return next_pcb->rsp;
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