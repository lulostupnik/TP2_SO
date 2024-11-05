// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <shell.h>


static void help();
static void kill_pid(char ** argv, uint64_t argc);
static void to_utc_minus_3 ( time_struct * time );
static void free_args(char ** args, uint64_t argc);
static void call_function_process(module m, char ** args, uint64_t argc);
static char ** command_parse(char shellBuffer[], uint64_t * argc);
static void interpret();
static void zoom_in();
static void zoom_out();
static void show_current_time();
static void get_regs();
static void shell_wait_pid(char ** args, uint64_t argc);
static void shell_nice(char **argv, uint64_t argc);
static void loop_process(char ** argv, uint64_t argc);
static void shell_block(char **argv, uint64_t argc);

static uint64_t font_size = 1;

#define BUILT_IN 1


void reader(){
	libc_pipe_open(0, READER);
	uint16_t buff[1000];
	char char_buff[1001];
	// if(libc_pipe_open(0, READER) == -1){
	// 	libc_fprintf(STDERR, "-1 as expected\n");
	// }
	while(1){
		sys_nano_sleep(100);
		
		int amount;
		amount = libc_pipe_read(0, buff, 999);
		if(amount == 0){
			libc_fprintf(STDERR, "EOF read: %s\n",amount, char_buff);
			libc_pipe_close(0);
			break;
		}
		int i=0;
		for( ;i<amount && i<1001;i++){
			char_buff[i] = (char) buff[i];
		}
		char_buff[i] = 0;
		libc_fprintf(STDERR, "I read %d bytes: %s\n",amount, char_buff);
	}
}
void writter(){
	libc_pipe_open(0, WRITER);
	int i = 0;
	while(1){
		uint16_t c = 0;
		//c = libc_get_char();
		uint16_t abc[] = {'1','2','3', '4', '5', '6', '7'};
		//char abc[] = "abcdefghijklmnopqrstuvwxyz";
		libc_pipe_write(0, abc, 3);

		if(i == 3){
			libc_pipe_close(0);
			break;
		}
		i++;
		//libc_pipe_write(0, abc, 20);
	}
}

static module modules[] = {
    {"help", "Muestra todos los módulos disponibles del sistema operativo.", help, BUILT_IN},
    {"time", "Muestra la hora actual del sistema.", show_current_time, BUILT_IN},
    {"eliminator", "Inicia el juego Eliminator, un clásico.", eliminator, BUILT_IN},
    {"zoomin", "Agranda los caracteres en pantalla.", zoom_in, BUILT_IN},
    {"zoomout", "Achica la pantalla.", zoom_out, BUILT_IN},
    {"getregs", "Muestra el estado actual de los registros.", get_regs, BUILT_IN},
    {"dividebyzero", "Genera una excepción de división por cero.", div0, BUILT_IN},
    {"opcode", "Genera una excepción de código de operación inválido.", op_code, BUILT_IN},
    {"clear", "Limpia la pantalla.", (void (*)(char **, uint64_t))libc_clear_screen, BUILT_IN},
    {"ipod", "Inicia el reproductor de música.", ipod_menu, BUILT_IN},
    {"loop", "Crea un proceso que imprime un saludo.", loop_process, !BUILT_IN},
    {"testprio", "Testea las prioridades del scheduler.", test_prio, !BUILT_IN},
    {"kill", "Mata al proceso con el número pid.", kill_pid, BUILT_IN},
    {"block", "Hace un swap entre ready y blocked para el proceso pid.", shell_block, BUILT_IN},
    {"wait", "Espera al proceso número pid.", shell_wait_pid, BUILT_IN},
    {"nice", "Cambia la prioridad del proceso pid a newprio.", shell_nice, BUILT_IN},
    {"testproc", "Testea la creación de procesos.", (void (*)(char **, uint64_t))test_processes, !BUILT_IN},
    {"testsync", "Testea la sincronización de procesos.", (void (*)(char **, uint64_t))test_sync, !BUILT_IN},
    {"testmm", "Testea el uso del malloc y free.", (void (*)(char **, uint64_t))test_mm, !BUILT_IN},
    {"ps", "Muestra información de los procesos.", libc_ps, BUILT_IN},
	{"reader", "Tests pipe reader.", reader, !BUILT_IN},
	{"writter", "Tests pipe writter. (use in foreground)", writter, !BUILT_IN}
};


int main()
{

	libc_set_font_size ( font_size );

	libc_puts ( WELCOME );
	help();
	while ( 1 ) {
		interpret();
	}

}

static void free_args(char ** args, uint64_t argc)
{
	for (int i = 0; i < argc; i++) {
		libc_free(args[i]);
	}
	libc_free(args);
	return;
}


static void call_function_process(module m, char ** args, uint64_t argc)
{
	if (m.is_built_in) {
		m.function(args, argc);
		free_args(args, argc);
		return;
	}

	uint8_t is_bckg = (libc_strcmp(args[argc - 1], "&") == 0);
	if(is_bckg){
		libc_free(args[argc - 1]);
		argc--;
	}

	int64_t ans = libc_create_process((main_function)m.function, LOW, args, argc);
	
	if (ans < 0) {
		libc_fprintf ( STDERR, "Error: Could not create process\n" );
	}else if (is_bckg){
		libc_printf("pid: %d in background\n", ans);
	}

	free_args(args, argc);
	if(!is_bckg) libc_wait(ans, NULL);
	return;
}


static char ** command_parse(char shellBuffer[], uint64_t * argc)
{
	char ** args = libc_malloc(MAX_ARGS * sizeof(char *));
	if (args == NULL) {
		*argc = -1;
		return NULL;
	}
	uint64_t args_count = 0;

	for (int i = 0; shellBuffer[i] != '\0';) {
		if (shellBuffer[i] == ' ') {
			i++;
			continue;
		}

		args[args_count] = libc_malloc(MAX_ARGS_SIZE * sizeof(char));

		if (args[args_count] == NULL) {
			for (int n = 0; n < args_count; n++) {
				libc_free(args[n]);
			}
			libc_free(args);
			*argc = -1;
			return NULL;
		}

		int j;
		for (j = 0; shellBuffer[i] != ' ' && shellBuffer[i] != '\0'; i++, j++) {
			args[args_count][j] = shellBuffer[i];
		}

		args[args_count][j] = '\0';
		args_count++;
	}

	*argc = args_count;

	if (args_count == 0) {
		libc_free(args);
		args = NULL;
	}

	return args;
}

static void interpret()
{
	libc_puts ( PROMPT );
	char shellBuffer[MAX_COMMAND_SIZE];
	libc_gets ( shellBuffer, MAX_COMMAND_SIZE );
	if ( libc_strlen ( shellBuffer ) == 0 ) {
		return;
	}
	char ** args;
	uint64_t argc;
	args = command_parse(shellBuffer, &argc);


	if (argc == -1) {
		libc_fprintf ( STDERR, "Not enough memory to create process\n" );
	}

	for ( int i = 0; i < MAX_MODULES && ((args != NULL) || (argc != 0)); i++ ) {
		if ( libc_strcmp (args[0], modules[i].name ) == 0 ) {
			call_function_process(modules[i], args, argc);
			return;
		}
	}

	libc_fprintf ( STDERR, "Invalid Command! Try Again >:(\n" );

}



static void kill_pid(char ** argv, uint64_t argc)
{
	pid_t pid;

	if (argc != 2 || argv == NULL || ((pid = satoi(argv[1])) < 0)) {
		libc_fprintf(STDERR, "Usage: killpid <pid>\n");
		return;
	}

	if (libc_kill(pid) < 0) {
		libc_fprintf(STDERR, "Error: Could not kill process %d\n", pid);
	}

}

static void shell_wait_pid(char ** args, uint64_t argc){
	if(argc != 2){
		libc_fprintf(STDERR, "Usage: wait <pid>\n");
		return;
	}
	pid_t pid;
	if((pid=satoi(args[1])) < 0 ){
		libc_fprintf(STDERR, "Error: pid must be positive\n");
		return;
	} 
	int64_t ret;
	pid_t ans_pid;
	ans_pid = libc_wait(pid, &ret);
	if(ans_pid == -1){
		libc_fprintf(STDERR, "Error: could not wait for pid %d\n", pid);
		return;
	}
	libc_printf("Pid %d returned %d\n", ans_pid, ret);
	return;
}

void help(char **args, uint64_t argc) {
    for (int i = 0; i < MAX_MODULES; i++) {
        libc_printf("- %s: %s\n", modules[i].name, modules[i].desc);
    }
}



static void zoom_in()
{
	if ( font_size < MAX_FONT_SIZE ) {
		font_size++;
		libc_set_font_size ( font_size );
	} else {
		libc_puts ( "Maximum font size reached!\n" );
	}
	return;
}


void zoom_out()
{
	if ( font_size > MIN_FONT_SIZE ) {
		font_size--;
		libc_set_font_size ( font_size );
	} else {
		libc_puts ( "Minimum font size reached!\n" );
	}
	return;
}


void show_current_time()
{
	time_struct time;
	libc_get_time ( &time );
	to_utc_minus_3 ( &time );
	libc_printf ( "%d/%d/%d [d/m/y]\n", time.day, time.month, time.year ); //Obs: En el PVS aparece como warning porque no implementamos %u (uint8_t)
	int64_t h = time.hour;
	libc_printf ( "%d:%d:%d [hour/min/sec] (Argentina)\n", h, time.minutes, time.seconds ); // la hora es -3 para que este en tiempo argentino.

	return;
}


static void to_utc_minus_3 ( time_struct * time )
{

	if ( time->hour < 3 ) {
		time->hour += 21;
		time->day--;
		if ( time->day == 0 ) {
			time->month--;
			if ( time->month == 0 ) {
				time->month = 12;
				time->year--;
			}
			if ( time->month == 2 ) {
				time->day = 28;
				if ( time->year % 4 == 0 ) {
					time->day = 29;
				}
			} else if ( time->month == 4 || time->month == 6 || time->month == 9 || time->month == 11 ) {
				time->day = 30;
			} else {
				time->day = 31;
			}
		}
	} else {
		time->hour = time->hour - 3;
	}
}


static void get_regs()
{
	libc_print_register_snapshot();
	return;
}



static void shell_nice(char **argv, uint64_t argc) {
    pid_t pid;
    if (argc != 3 || ((pid = satoi(argv[1])) < 0)) {
        libc_fprintf(STDERR, "Usage: nice <pid> <new_prio>");
        return;
    }
    priority_t prio;
    if (libc_strnocasecmp(argv[2], "low") == 0) {
        prio = LOW;
    } else if (libc_strnocasecmp(argv[2], "medium") == 0) {
        prio = MEDIUM;
    } else if (libc_strnocasecmp(argv[2], "high") == 0) {
        prio = HIGH;
    } else {
        libc_fprintf(STDERR, "Invalid priority. Use 'low', 'medium', or 'high' for <newprio>.\n");
        return;
    }
    libc_nice(pid, prio);
}


static void shell_block(char **argv, uint64_t argc){
	pid_t pid;
    if (argc != 2 || ((pid = satoi(argv[1])) < 0)) {
        libc_fprintf(STDERR, "Usage: block <pid>");
        return;
    }
	int8_t status = libc_get_status(pid);
	if(	(status != BLOCKED) && (status != READY) ){
		libc_fprintf(STDERR, "Error: The status of pid %d neither ready or blocked \n", pid);
        return;
	}
	if(status == BLOCKED){
		libc_unblock(pid);
	}else if(status == READY){
		libc_block(pid);
	}
}

static void loop_process(char ** argv, uint64_t argc){
	pid_t pid = libc_get_pid();
	while(1){
		sys_nano_sleep((pid^10) + pid*10 + 100);
		libc_printf("\nHello my friend, my pid is %d, I hope you are well\n", pid);
	}
}