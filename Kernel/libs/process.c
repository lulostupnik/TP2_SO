
#include <process.h>

#define STACK_SIZE 0x0FFF
#define PCB_AMOUNT 100



PCB pcb_array[PCB_AMOUNT] = {0}; 
uint64_t amount_of_processes = 0;

// todo: agregar cantidad de procesos. Es decir, "cantidad de lugares ocupados"


static int64_t find_free_pcb();
static char ** copy_argv(int64_t pid, char ** argv, uint64_t argc);

//extern uint64_t strlen ( const char *str );
static uint64_t my_strlen ( const char *str )  //@todo poner en otro archivo
{
	const char *s = str;
	while ( *s )
		++s;
	return s - str;
}


static char ** copy_argv(int64_t pid, char ** argv, uint64_t argc){
   
    if( (argc == 0 && argv != NULL) || (argc>0 && argv==NULL)){ //@todo check
        return NULL;
    }

    if(argc == 0){
        return NULL;
    }
    
    char ** ans = my_malloc(sizeof(char *) * (argc+1));

    if(ans == NULL){
        return NULL;
    }

    for(uint64_t i=0; i<argc;i++){
        uint64_t len = my_strlen(argv[i])+1;
        char * p = my_malloc(len);
        if(p == NULL){ //@TODO check y modularizar
            for(uint64_t j=0; j<i;j++){
                my_free(ans[j]);
            }
            my_free(ans);
            return NULL;
        }
        memcpy(p, argv[i], len);
        ans[i] = p;
    }
    return ans;
}


int64_t new_process(main_function rip, priority_t priority, uint8_t killable,char ** argv, uint64_t argc){
  
    if(!((priority != LOW) || (priority != MEDIUM) || (priority != HIGH))){
        return -1;
    }
    
    int64_t pid = find_free_pcb();
    if(pid == -1){
        return -1;
    }

    uint64_t rsp_malloc = (uint64_t) my_malloc(STACK_SIZE) ;
    uint64_t rsp = rsp_malloc + STACK_SIZE;

    if(rsp_malloc == NULL){  
        return -1;
    }

    char ** args_cpy = copy_argv(pid, argv, argc);
    if(argc>0 && args_cpy == NULL){
        my_free((void *)rsp_malloc); 
        pcb_array[pid].status = FREE;
        return -1;
    }
    
    rsp = load_stack(rip, rsp, args_cpy, argc, pid);
    
   

    
    pcb_array[pid].pid = pid;
   // pcb_array[pid].ppid = running->pid;
    pcb_array[pid].rsp = rsp;
    pcb_array[pid].status = READY;
    pcb_array[pid].argv = args_cpy;
    pcb_array[pid].argc = argc;
    pcb_array[pid].priority = priority;
    pcb_array[pid].base_pointer = rsp_malloc;
    pcb_array[pid].killable = killable;

    ready(&pcb_array[pid]);   
    amount_of_processes++;
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

// int64_t kill_process_ret(int64_t pid, int64_t ret){
//     if(pid >= PCB_AMOUNT || pid < 0 || pcb_array[pid].status == FREE){
//         return -1;
//     }
//     unschedule(&pcb_array[pid]);
//     pcb_array[pid].status = ZOMBIE;
//      pcb_array[pid].ret = ret;
// }
void set_free_pcb(int64_t pid){
    PCB * process = get_pcb(pid);
    if(process == NULL){
        return;
    }
    my_free(process->base_pointer);
    if(process->argv == NULL){
        process->status  = FREE;
        return;
    }
    for(uint64_t i=0; i<process->argc ; i++){
        my_free(process->argv[i]);
    }
    my_free(process->argv);
    process->status = FREE;
    
}


int64_t kill_process(int64_t pid){
    if(pid >= PCB_AMOUNT || pid < 0 || pcb_array[pid].status == FREE || !pcb_array[pid].killable ){
        return -1;
    }

    unschedule(&pcb_array[pid]);
    set_free_pcb(pid);
    amount_of_processes--;
    return 0;
    // llamar a int20 si es el proceso que está corriendo?
}

void get_process_info(PCB * pcb, process_info * process){
    process->pid = pcb->pid;
    process->ppid = pcb->ppid;
    process->priority = pcb->priority;
    // process->base_pointer = pcb->rbp;
    // todo BASE POINTER !!
    process->stack_pointer = pcb->rsp;
    process->status = pcb->status;
    // process->foreground = pcb->foreground;
}

process_info_list * ps(){
    process_info_list * process_list = my_malloc(sizeof(process_info_list));
    if(process_list == 0){
        return 0; // todo NULL
    }
    process_list->amount_of_processes = amount_of_processes;
    process_info * processes = my_malloc(amount_of_processes * sizeof(process_info));
    if(processes == 0){
        my_free((void *) process_list);
        return 0; // todo NULL
    }

    for(int i = 0, found = 0; i < PCB_AMOUNT && found < amount_of_processes; i++){
        if(pcb_array[i].status != FREE){
            get_process_info(&pcb_array[i], &processes[found]);
            found++;
        }
    }
    // if(found != amount_of_processes){/*ERROR*/}
    
    process_list->processes = processes;
    return process_list;

}
// IDEAS FUTURO:
// freePIDList o arreglo o lo que sea

// void list_processes(){
//     vdriver_text_write("---------------------------------------------------------\n");
//     vdriver_text_write("| %-5s | %-8s | %-18s |\n", "PID", "Priority", "Stack Pointer");
//     vdriver_text_write("---------------------------------------------------------\n");
//     for (int i = 0; i < PCB_AMOUNT; i++) {
//         if (pcb_array[i].status != FREE) {
//             vdriver_text_write("| %-5d | %-8d | %-18p |\n", pcb_array[i].pid, pcb_array[i].priority, (void *)pcb_array[i].rsp); // @TODO: Agregarle nombre, base pointer y foreground
//         }
//     }
//     vdriver_text_write("---------------------------------------------------------\n");
// }