// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <shell.h>


static void help();

static void kill_pid(char ** argv, uint64_t argc);

static void toUtcMinus3 ( time_struct * time );

static uint64_t font_size = 1;

#define BUILT_IN 1



static module modules[] = {
{"help", help, BUILT_IN},
{"time", showcurrentTime, BUILT_IN},
{"eliminator", eliminator, BUILT_IN},
{"zoomin", zoomIn, BUILT_IN},
{"zoomout", zoomOut, BUILT_IN},
{"getregs", getRegs, BUILT_IN},
{"dividebyzero", div0, BUILT_IN},
{"opcode", op_code, BUILT_IN},
{"clear", (void (*)(char **, uint64_t)) libc_clear_screen, BUILT_IN},
{"ipod", ipod_menu, BUILT_IN},
{"testmm", (void (*)(char **, uint64_t)) test_mm, !BUILT_IN},
{"testprio", test_prio, !BUILT_IN},
{"testproc", (void (*)(char **, uint64_t)) test_processes, !BUILT_IN},
{"killpid", kill_pid, BUILT_IN}
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

void free_args(char ** args, uint64_t argc)
{
	for (int i = 0; i < argc; i++) {
		libc_free(args[i]);
	}
	libc_free(args);
	return;
}

void call_function_process(module m, char ** args, uint64_t argc)
{
	if (m.is_built_in) {
		m.function(args, argc);
		free_args(args, argc);
		return;
	}

	int64_t ans = sys_create_process((main_function)m.function, LOW, args, argc);
	if (ans < 0) {
		libc_fprintf ( STDERR, "Could not create process\n" );
	}

	free_args(args, argc);
	return;
}

char ** command_parse(char shellBuffer[], uint64_t * argc)
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


void interpret()
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
	int64_t pid;

	if (argc != 2 || argv == NULL || ((pid = satoi(argv[1])) < 0)) {
		libc_fprintf(STDERR, "Usage: killpid <pid>\n");
		return;
	}

	if (libc_kill(pid) < 0) {
		libc_fprintf(STDERR, "Could not kill process %d\n", pid);
	}

}

static void help(char ** args, uint64_t argc)
{

	libc_puts ( "\nComandos disponibles:\n\n" );
	libc_puts ( "- help: Muestra todos los modulos disponibles del sistema operativo.\n" );
	libc_puts ( "- time: Muestra la hora actual del sistema.\n" );
	libc_puts ( "- eliminator: Inicia el juego Eliminator, un clasico.\n" );
	libc_puts ( "- zoomin: Agranda los caracteres en pantalla.\n" );
	libc_puts ( "- zoomout: Achica  la pantalla.\n" );
	libc_puts ( "- getregs: Muestra el estado actual de los registros.\n" );
	libc_puts ( "- dividebyzero: Genera una excepcion de division por cero.\n" );
	libc_puts ( "- opcode: Genera una excepcion de codigo de operacion invalido.\n" );
	libc_puts ( "- clear: Limpia la pantalla.\n" );
	libc_puts ( "- ipod: Inicia el reproductor de musica.\n" );
	libc_puts ( "- testprio: Testea las prioridades del scheduler.\n" );
	libc_puts ( "- killpid <pid>: Mata al pid numero pid.\n" );
	libc_puts ( "- testproc <maxprocesses>: Testea la creacion de procesos.\n" );
	libc_puts ( "- testmm <maxmemory>: Testea el uso del malloc y free.\n\n" );

}



void zoomIn()
{
	if ( font_size < MAX_FONT_SIZE ) {
		font_size++;
		libc_set_font_size ( font_size );
	} else {
		libc_puts ( "Maximum font size reached!\n" );
	}
	return;
}

void zoomOut()
{
	if ( font_size > MIN_FONT_SIZE ) {
		font_size--;
		libc_set_font_size ( font_size );
	} else {
		libc_puts ( "Minimum font size reached!\n" );
	}
	return;
}


void showcurrentTime()
{
	time_struct time;
	sys_get_time ( &time );
	toUtcMinus3 ( &time );
	libc_printf ( "%d/%d/%d [d/m/y]\n", time.day, time.month, time.year ); //Obs: En el PVS aparece como warning porque no implementamos %u (uint8_t)
	int64_t h = time.hour;
	libc_printf ( "%d:%d:%d [hour/min/sec] (Argentina)\n", h, time.minutes, time.seconds ); // la hora es -3 para que este en tiempo argentino.

	return;
}


static void toUtcMinus3 ( time_struct * time )
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


void getRegs()
{
	libc_print_register_snapshot();
	return;
}

