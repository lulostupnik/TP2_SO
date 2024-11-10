// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <programs.h>

int64_t loop(char ** argv, int argc){
    if(argc != 2){
        libc_fprintf(STDERR, "Usage: loop <sleep_ticks>\n");
        return 1;
    }
    int64_t satoi_flag;
    int64_t seconds = libc_satoi(argv[1] , &satoi_flag);
    if(seconds < 0 || !satoi_flag){
        libc_fprintf(STDERR, "Error: Invalid amount of seconds\n");
        return 1;
    }
    uint64_t ticks = seconds * 18;
	int64_t ans = 1;
    while(ans > 0){
        libc_ticks_sleep(ticks);
		ans = libc_printf("Hello! I'm a loop with pid: %d\n", libc_get_pid());
    }
	return 0;
}

int64_t wc(char ** argv, int argc){
    if(argc != 1){
        libc_fprintf(STDERR, "Usage: wc\n");
        return 1;
    }
	char c;
    char prev = 0;
    uint64_t lines = 1;
	while ( (c = libc_get_char()) != 0 ) {
        libc_put_char(c);
        if(c == '\n'){
            lines = lines + 1;
        }
        prev = c;
	}

    if(prev == 0 || prev == '\n'){
        lines--;
    }
    libc_printf("\nLines: %d\n", lines);
	return 0;
}


int64_t filter(){
	char ans;
	while((ans = libc_get_char()) > 0 ){
		if(!(ans == 'a' || ans == 'A' || ans == 'e' || ans == 'E' || ans == 'i' || ans == 'I' || ans == 'o' || ans =='O' || ans == 'u' || ans == 'U')){
			libc_printf("%c", ans);
		}
	}
	return 0;
}

int64_t cat(){
	char ans;
	while((ans = libc_get_char()) > 0 ){
		libc_printf("%c", ans);
	}
	libc_printf("\n");
	return 0;
}


int64_t ps_program(){

	process_info_list * process_list = sys_ps();
	if(process_list == NULL){
		libc_fprintf(STDERR, "Error while getting process list\n");
		return 1;
	}
	libc_printf("Amount of processes: %d\n", process_list->amount_of_processes);
    libc_printf("PID | Ground      | Prio  | Stack Base Ptr| Last Stack Addr |    RSP     | Status | STDOUT| STDERR| STDIN | Name       \n");
    libc_printf("----|-------------|-------|---------------|-----------------|------------|--------|-------|-------|-------|-----------\n");

    for (int i = 0; i < process_list->amount_of_processes; i++) {
		
		if( process_list->processes[i].pid < 10){
			libc_printf("%d   | ", process_list->processes[i].pid);  
		}else{
			 libc_printf("%d  | ", process_list->processes[i].pid);  
		}
                                     
        libc_printf("%s  | ", process_list->processes[i].is_background ? "Background" : "Foreground");     
        libc_printf("%s| ", process_list->processes[i].priority == LOW ? "Low   ":(process_list->processes[i].priority == MEDIUM ? "Medium":"High  " )); 
        libc_printf("0x%x    | ", process_list->processes[i].lowest_stack_address + STACK_SIZE); 
        libc_printf("0x%x      | ", process_list->processes[i].lowest_stack_address);        
        libc_printf("0x%x | ", process_list->processes[i].stack_pointer);          
        libc_printf("%s| ", process_list->processes[i].status == READY ? "Ready  ":(process_list->processes[i].status == ZOMBIE ? "Zombie ":"Blocked"));
	
		for(int j = 0; j < CANT_FDS; j++){
			char * fds_print[] = {"STDOUT", "STDERR", "STDIN "};
			if(process_list->processes[i].fds[j] == -1){
				libc_printf("  -   | ");
			}else if(process_list->processes[i].fds[j] >= 0 && process_list->processes[i].fds[j] <= MAX_COMMON_FD){
				libc_printf("%s| ", fds_print[process_list->processes[i].fds[j]]);
			}else{
				int to_print =  process_list->processes[i].fds[j];
				if(to_print < 10){
					libc_printf("  %d   | ", to_print);
				}else{
					libc_printf("  %d  | ", to_print);
				}
			}
		}            
		libc_printf("%s\n", process_list->processes[i].name ? process_list->processes[i].name : "No name"); 

    }
	sys_free_ps(process_list);
	return 0;
}

int64_t mem(){
	memory_info info;
	if(sys_mem_info(&info) == -1){
		libc_fprintf(STDERR, "Error while getting memory info\n");
		return 1;
	}
	libc_printf("Total memory: %x\n", info.total_size);
	libc_printf("Free memory: %x\n", info.free);
	return 0;
}