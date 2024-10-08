#include <listADT.h>
#include <process.h>
uint8_t initialized = 0;

static listADT ready_list;
static listADT blocked_list;
static PCB * running = NULL;

int compare_elements(elemTypePtr e1, elemTypePtr e2) {
    return e1 - e2; 
}

void initialize_scheduler(){
    tCompare cmp = compare_elements;
    ready_list = newList(cmp);
    blocked_list = newList(cmp);
    initialized = 1;
    return;
}

void ready(PCB * process){
    process->status = READY;
    addList(ready_list, process);
    deleteList(blocked_list, process);

}   

void block(PCB * process){
    process->status = BLOCKED;
    deleteList(ready_list, process);
    addList(blocked_list, process);

}

uint64_t scheduler(uint64_t current_rsp){
    return current_rsp;

    /*uint64_t new_rsp;
    if(running!=NULL){
        running->rsp = current_rsp;
    }
    new_rsp=next(ready_list)->rsp;

    return new_rsp;*/
}



    /*
    if(isEmptyList(ready_list)){
        //while(1){_hlt();}
        return; 
    }
    if(running==NULL){
        listADT(toBegin(ready_list));
        running = next(ready_list);
        running->status = RUNNING;
        return;
    }
    
    if(running->status == RUNNING){
        running->status = READY;
        addList(ready_list, running);
    }
    else if(running->status == BLOCKED){
        deleteList(ready_list, running);
        addList(blocked_list, running);
    }

    PCB * next_process = NULL;

    deleteList(ready_list, next_process);
    next_process->status = RUNNING;
    context_switch(running, next_process);
    running=next_process;*/