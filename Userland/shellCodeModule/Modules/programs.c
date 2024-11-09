#include <programs.h>

void loop(char ** argv, int argc){
    if(argc != 2){
        libc_printf("Usage: loop <seconds>\n");
        return;
    }
    int64_t seconds = satoi(argv[1]);
    if(seconds < 0){
        libc_printf("Invalid amount of seconds\n");
        return;
    }
    uint64_t ticks = seconds * 18;
    while(1){
        sys_nano_sleep(ticks);
        libc_printf("Hello! I'm a loop with pid: %d\n", libc_get_pid());
    }
}
/*
void cat(){
	char buff[1000];
	int amount = 0;
	while((amount = sys_read(buff, 999)) > 0 ){
		buff[amount] = 0;
		libc_fprintf(STDERR, "%s", buff);
	}
	libc_pipe_close(3);
}
*/
/*
void wc(char ** argv, int argc){
    if(argc != 1){
        libc_printf("Usage: wc\n");
        return;
    }
	uint16_t c[10];
    uint16_t prev = EOF;
    uint64_t lines = 1;
	while ( ( sys_read(c, 1) ) == 1 ) {
        libc_put_char((char) c[0]);
        if(c[0] == '\n'){
            lines = lines + 1;
            // libc_printf("Lines:   uuu %d\n", lines);
        }
        prev = c[0];
        libc_printf("prev: %c\n", prev);
	}
    libc_printf("prev: %c\n", prev);
    if(prev == EOF || prev == '\n'){
        lines--;
    }
    libc_printf("Lines: %d\n", lines);

	return;
}
*/
void wc(char ** argv, int argc){
    if(argc != 1){
        libc_printf("Usage: wc\n");
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
        // libc_printf("prev: %c\n", prev);
	}

    libc_printf("prev: %c\n", prev);
    if(prev == 0 || prev == '\n'){
        lines--;
    }
    libc_printf("Lines: %d\n", lines);

	return;
}


void filter(){
    return;
}

/*
void cat(){
}
*/
