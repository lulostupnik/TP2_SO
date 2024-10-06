/*#include "listADT.h"
#include "process.h"
uint_8 initialized = 0;

static listADT ready_list;
static listADT blocked_list;
static PCB * running = NULL;

int compare_elements(elemTypePtr e1, elemTypePtr e2) {
    return e1->pid - e2->pid; 
}

void context_switch(PCB old_process, PCB new_process) {
    //~TODO Implementar Funciones
    //save_context(old_process);

    //load_context(new_process);
    return;
}

uint8_t find_first_ready(listADT list, PCB * next_process){
    uint8_t found = 0;

    while (hasNext(ready_list) && !found) {
        PCB *process = next(ready_list);
        if (process->status == READY) {
            *next_process = process;  
            found = 1;
        }
    }

    if (!found) {
        toBegin(ready_list);
        while (hasNext(ready_list) && !found) {
            PCB *process = next(ready_list);
            if (process->status == READY) {
                *next_process = process;
                found = 1;
            }
        }
    }

    return found; 
}

void initialize_scheduler(){
    tCompare cmp = compare_elements;
    ready_list = newList(cmp);
    blocked_list = newList(cmp);
    initialized = 1;
    return;
}

void scheduler(){
    if(!initialized){
        initialize_scheduler();
    }

    if(isEmptyList(ready_list)){
        return;
    }
    if(running==NULL){
        listADT(toBegin(ready_list));
        running = next(ready_list);
        running->status = RUNNING;
        return;
    }
    else{
        if(running->status == RUNNING){
            running->status = READY;
            addList(ready_list, running);
        }
        else if(running->status == BLOCKED){
            deleteList(ready_list, running);
            addList(blocked_list, running);
        }
    }

    PCB * next_process = NULL;

    if(find_first_ready(ready_list, &next_process)){
        deleteList(ready_list, next_process);
        next_process->status = RUNNING;
        context_switch(running, next_process);
        running=next_process;
    }
}*/