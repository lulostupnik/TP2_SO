
#include <process.h>

#define STACK_SIZE 256

#define PCB_AMOUNT 100



PCB pcb_array[PCB_AMOUNT] = {0}; //@todo va aca?


static int64_t find_free_pcb();
// static int copy_argv(uint64_t pid, char ** argv, uint64_t argc);
static char ** copy_argv(uint64_t pid, char ** argv, uint64_t argc);

//extern uint64_t strlen ( const char *str );
static uint64_t my_strlen ( const char *str )  //@todo poner en otro archivo
{
	const char *s = str;
	while ( *s )
		++s;
	return s - str;
}


static char ** copy_argv(uint64_t pid, char ** argv, uint64_t argc){
   
    if( (argc == 0 && argv != NULL) || (argc>0 && argv==NULL)){ //@todo check
        return NULL;
    }

   /// pcb_array[pid].argc = argc;
    
    if(argc == 0){
       // pcb_array[pid].argv == NULL;
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


int64_t new_process(main_function rip, priority_t priority, char ** argv, uint64_t argc){
  
   
    
    // uint64_t rsp = (uint64_t) my_malloc(STACK_SIZE);
    // if(rsp == NULL){
    //     return -1;
    // }
    // rsp += STACK_SIZE;
    int64_t pid = find_free_pcb();
    if(pid == -1){
        return -1;
    }

    uint64_t rsp_malloc = (uint64_t) my_malloc(STACK_SIZE*4) ;  //@TODO No hacerlo dinamico. Definir zona de STACKS y hacer un arreglo
    uint64_t rsp = rsp_malloc + STACK_SIZE*4;

    if(rsp_malloc == NULL){  //Este checkeo con la zona estatica no lo tendriamos que hacer
        return -1;
    }

    // todo -> cambiar el BLOCK_SIZE en el memory manager para que entre el stack
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


// int64_t kill_process_ret(int64_t pid, int64_t ret){
//     if(pid >= PCB_AMOUNT || pid < 0 || pcb_array[pid].status == FREE){
//         return -1;
//     }
//     unschedule(&pcb_array[pid]);
//     pcb_array[pid].status = ZOMBIE;
//      pcb_array[pid].status = ret;
// }



int64_t kill_process(int64_t pid){
    if(pid >= PCB_AMOUNT || pid < 0 || pcb_array[pid].status == FREE){
        return -1;
    }
    unschedule(&pcb_array[pid]);
    pcb_array[pid].status = ZOMBIE;
}



// IDEAS FUTURO:
// freePIDList o arreglo o lo que sea
