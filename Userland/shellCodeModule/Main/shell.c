// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <shell.h>


static void help();

static void toUtcMinus3 ( time_struct * time );

static uint64_t font_size = 1; // font_size 1 is the default size


static module modules[] = {
{"help", help},
{"time", showcurrentTime},
{"eliminator", eliminator},
{"zoomin", zoomIn},
{"zoomout", zoomOut},
{"getregs", getRegs},
{"dividebyzero", div0},
{"opcode", op_code},
{"clear", clear},
{"testmm", start_test_mm},
{"ipod", ipod_menu},
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
void interpret()
{
	puts ( PROMPT );
	char shellBuffer[MAX_COMMAND_SIZE];
	gets ( shellBuffer, MAX_COMMAND_SIZE );
	if ( strlen ( shellBuffer ) == 0 ) {
		return;
	}
	for ( int i = 0; i < MAX_MODULES; i++ ) {
		if ( strcmp ( shellBuffer, modules[i].name ) == 0 ) {
			modules[i].function();
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

