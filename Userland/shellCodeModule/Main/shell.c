// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <shell.h>


static void help();

static void toUtcMinus3 ( time_struct * time );

static uint64_t font_size = 1; // font_size 1 is the default size

#define BUILT_IN 1


static module modules[] = {
{"help", help, BUILT_IN, 0},
{"time", showcurrentTime, BUILT_IN, 0},
{"eliminator", eliminator, BUILT_IN, 0},
{"zoomin", zoomIn, BUILT_IN, 0},
{"zoomout", zoomOut, BUILT_IN, 0},
{"getregs", getRegs, BUILT_IN, 0},
{"dividebyzero", div0, BUILT_IN, 0},
{"opcode", op_code, BUILT_IN, 0},
{"clear", clear, BUILT_IN, 0},
{"ipod", ipod_menu, BUILT_IN, 0},
{"testmm", test_mm, !BUILT_IN,  0}, 
{"testprio",test_prio, !BUILT_IN, 0},
{"testproc",test_processes, !BUILT_IN, 0}
};


int main()
{

	setFontSize ( font_size );

	puts ( WELCOME );
	help();
	

	while ( 1 ) {
		interpret();
	}

}

void call_function_process(module m, char** args, uint64_t argc)
{
	if(m.is_built_in){
		m.function();
		return;
	}

	int64_t ans = sys_create_process(m.function, m.priority, args, argc); //@todo le agregamos checkeo??
	if(ans < 0){
		fprintf ( STDERR, "Could not create process\n" );
	}
	
	for(int i = 0; i < argc; i++){
		my_free(args[i]);
	}

	my_free(args);
	return;
}

char ** command_parse(char shellBuffer[], uint64_t * argc)
{
	char ** args = my_malloc(MAX_ARGS * sizeof(char *));
	if(args == NULL){
		*argc = -1;
		return NULL;
	}
	uint64_t args_count = 0;

	for (int i = 0; shellBuffer[i] != '\0';) {
		if(shellBuffer[i] == ' '){
			i++;
			continue;
		}

        args[args_count] = my_malloc(MAX_ARGS_SIZE * sizeof(char));

		if(args[args_count] == NULL){
			for(int n = 0; n < args_count; n++){
				my_free(args[n]);
			}
			my_free(args);
			*argc = -1;
			return NULL;
		}

		int j;
		for(j = 0; shellBuffer[i] != ' ' && shellBuffer[i] != '\0'; i++, j++){
			args[args_count][j] = shellBuffer[i];
		}
		
        args[args_count][j] = '\0';
        args_count++;
    }

	*argc = args_count;

	if(args_count == 0){
		my_free(args);
		args = NULL;
	}
	
	return args;
}

	/*

    // Parse the commandBuffer
    for (int i = 0; commandBuffer[i] != '\0';) {

        // Skip spaces
        while (commandBuffer[i] == ' ') {
            i++;
        }

        // If we reached the end of the string, break
        if (commandBuffer[i] == '\0') {
            break;
        }

        // If we found an argument, copy it to a new array
        args[argsDim] = sysmalloc(MAX_ARG_SIZE * sizeof(char));
        int j = 0;
        while (commandBuffer[i] != ' ' && commandBuffer[i] != '\0') {
            args[argsDim][j] = commandBuffer[i];
            i++;
            j++;
        }
        args[argsDim][j] = '\0';
        args[argsDim] = sysrealloc(args[argsDim], (j + 1) * sizeof(char));
        argsDim++;
    }

    // If the last argument is an ampersand, remove it and set flag
    if (strcmp(args[argsDim - 1], "&") == 0) {
        sysfree(args[argsDim - 1]);
        args[argsDim - 1] = NULL;
        argsDim--;
        *isBackground = TRUE;
    } else {
        *isBackground = FALSE;
    }

    // If we found no arguments, free the array
    // Else reallocate the array to the correct size
    if (argsDim == 0) {
        sysfree(args);
        args = NULL;
    } else {
        args = sysrealloc(args, argsDim * sizeof(char *));
    }

    *argc = argsDim;
    return args;
}
*/

void interpret()
{
	puts ( PROMPT );
	char shellBuffer[MAX_COMMAND_SIZE];
	gets ( shellBuffer, MAX_COMMAND_SIZE );
	if ( strlen ( shellBuffer ) == 0 ) {
		return;
	}
	char ** args;
	uint64_t argc;
	args = command_parse(shellBuffer, &argc);

	if(argc == -1){
		fprintf ( STDERR, "Not enough memory to create process\n" );
	}

	for ( int i = 0; i < MAX_MODULES && ((args != NULL) || (argc != 0)); i++ ) {
		if ( strcmp (args[0], modules[i].name ) == 0 ) {
			//modules[i].function();
			call_function_process(modules[i], args, argc);
			return;	
		}
	}
	
	fprintf ( STDERR, "Invalid Command! Try Again >:(\n" );

}

static void help()
{
	puts ( "\nComandos disponibles:\n\n" );
	puts ( "- help: Muestra todos los modulos disponibles del sistema operativo.\n" );
	puts ( "- time: Muestra la hora actual del sistema.\n" );
	puts ( "- eliminator: Inicia el juego Eliminator, un clasico.\n" );
	puts ( "- zoomin: Agranda los caracteres en pantalla.\n" );
	puts ( "- zoomout: Achica  la pantalla.\n" );
	puts ( "- getregs: Muestra el estado actual de los registros.\n" );
	puts ( "- dividebyzero: Genera una excepcion de division por cero.\n" );
	puts ( "- opcode: Genera una excepcion de codigo de operacion invalido.\n" );
	puts ( "- clear: Limpia la pantalla.\n" );
	puts ( "- ipod: Inicia el reproductor de musica.\n" );
	puts ( "- testprio: Testea las prioridades del scheduler.\n" );
	puts ( "- testproc: Testea la creacion de procesos.\n" );
	puts ( "- testmm: Testea el uso del malloc y free.\n\n" );

}



// Function to zoom in
void zoomIn()
{
	if ( font_size < MAX_FONT_SIZE ) {
		font_size++;
		setFontSize ( font_size );
	} else {
		puts ( "Maximum font size reached!\n" );
	}
	return;
}

// Function to zoom out
void zoomOut()
{
	if ( font_size > MIN_FONT_SIZE ) {
		font_size--;
		setFontSize ( font_size );
	} else {
		puts ( "Minimum font size reached!\n" );
	}
	return;
}




void showcurrentTime()
{
	time_struct time;
	sys_get_time ( &time );
	toUtcMinus3 ( &time );
	printf ( "%d/%d/%d [d/m/y]\n", time.day, time.month, time.year ); //Obs: En el PVS aparece como warning porque no implementamos %u (uint8_t)
	int64_t h = time.hour;
	printf ( "%d:%d:%d [hour/min/sec] (Argentina)\n", h, time.minutes, time.seconds ); // la hora es -3 para que este en tiempo argentino.

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
	print_register_snapshot();
	return;
}

void clear()
{
	clear_screen();
}

