#include <programs.h>

void loop(char ** argv, int argc){
    if(argc != 2){
        libc_fprintf(STDERR, "Usage: loop <seconds>\n");
        return;
    }
    int64_t satoi_flag;
    int64_t seconds = satoi(argv[1] , &satoi_flag);
    if(seconds < 0 || !satoi_flag){
        libc_fprintf(STDERR, "Invalid amount of seconds\n");
        return;
    }
    uint64_t ticks = seconds * 18;
    while(1){
        sys_ticks_sleep(ticks);
        libc_printf("Hello! I'm a loop with pid: %d\n", libc_get_pid());
    }
}

void wc(char ** argv, int argc){
    if(argc != 1){
        libc_fprintf(STDERR, "Usage: wc\n");
        return;
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
    libc_printf("Lines: %d\n", lines);

	return;
}


void filter(){
	char ans;
	while((ans = libc_get_char()) > 0 ){
		if(!(ans == 'a' || ans == 'A' || ans == 'e' || ans == 'E' || ans == 'i' || ans == 'I' || ans == 'o' || ans =='O' || ans == 'u' || ans == 'U')){
			libc_printf("%c", ans);
		}
	}

}

/*
void cat(){
}
*/
