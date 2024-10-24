// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <libc.h>



/**
 * @brief Reads a character from the standard input.
 *
 * This function uses the read system call to read a character from the standard input (STDIN) (the keyboard buffer)
 * It blocks until a character is available.
 *
 * @return int64_t Returns the value of the read character.
 */
char libc_get_char()
{
	uint16_t c;
	while ( sys_read ( STDIN, &c, 1 ) == 0 || c > 255 );
	return ( char ) c;
}



/**
 * @brief Writes a character to the standard output.
 *
 * This function uses the sys_write system call to write a character to the standard output (STDOUT)
 * (The character is written to the screen)
 *
 * @param c The character to write.
 */
void libc_put_char ( char c )
{
	sys_write ( STDOUT, &c, 1 );
}



/**
 * @brief Generates a libc_beep sound.
 *
 * This function uses the sys_beep system call to generate a libc_beep sound.
 * The libc_beep sound is produced by the system speaker.
 *
 * @param frequency The frequency of the libc_beep sound in hertz.
 * @param duration The duration of the libc_beep sound in milliseconds.
 * @return int64_t Returns 0 if the libc_beep was successfully generated, or -1 if an error occurred.
 */
int64_t libc_beep ( uint64_t frequency, uint64_t duration )
{
	return sys_beep ( frequency, duration );
}



/**
 * @brief Clears the system screen.
 *
 * This function uses the sys_clear_screen system call to clear the system screen.
 * The screen is cleared to black.
 *
 * @return int64_t Returns 0 if the screen was successfully cleared, or -1 if an error occurred.
 */
int64_t libc_clear_screen()
{
	return sys_clear_screen();
}



/**
 * @brief Sets the font size of the system console.
 *
 * This function uses the sys_set_font_size system call to set the font size of the system console.
 * The font size is set to the specified size.
 *
 * @param size The desired font size.
 * @return int64_t Returns 0 if the font size was successfully set, or -1 if an error occurred.
 */
int64_t libc_set_font_size ( uint64_t size )
{
	return sys_set_font_size ( size );
}



/**
 * @brief Calculates the length of a string.
 *
 * This function calculates the length of the null terminated string pointed to by `str`, excluding the terminating null byte ('\0').
 *
 * @param str The string whose length is to be calculated.
 * @return size_t Returns the number of characters in the string pointed to by `str`.
 */
uint64_t libc_strlen ( const char * str )
{
	const char * s = str;
	while ( *s )
		++s;
	return s - str;
}



/**
 * @brief Converts a number to a string representation in a specified base.
 *
 * This function converts a number to its string representation in a specified base.
 * It uses a static buffer to hold the result, so the returned string should be used or copied before the next call to `libc_num_to_string`.
 *
 * @param num The number to be converted.
 * @param base The base to use for the conversion. This should be between 2 and 16 inclusive.
 * @return char* Returns a pointer to the string representation of the number. This string is null-terminated.
 */
// char * libc_num_to_string ( uint64_t num, uint64_t base )
// {
// 	static char buffer[64];
// 	char * ptr = &buffer[63];
// 	*ptr = '\0';
// 	do {
// 		*--ptr = "0123456789ABCDEF"[num % base];
// 		num /= base;
// 	} while ( num != 0 );
// 	return ptr;
// }

char * libc_num_to_string(uint64_t num, uint64_t base, char *buffer, size_t buffer_size) {
    if (base < 2 || base > 16 || buffer_size < 2) {
        return NULL;  
    }

    char *ptr = &buffer[buffer_size - 1];  
    *ptr = '\0';  

    do {
        if (ptr == buffer) {
            return NULL;  
        }
        *--ptr = "0123456789ABCDEF"[num % base];
        num /= base;
    } while (num != 0);

    return ptr;  
}




/**
 * @brief Writes a string to the standard output.
 *
 * This function uses the sys_write system call to write a string to the standard output (STDOUT).
 * The string is written to the screen. A newline character is appended at the end of the string.
 *
 * @param str The string to write.
 * @return int64_t Returns the number of characters written if the operation was successful, or -1 if an error occurred.
 */
int64_t libc_puts ( const char * str )
{
	return sys_write ( STDOUT, str, libc_strlen ( str ) );
}



/**
 * @brief Writes a character to a file descriptor.
 *
 * This function uses the sys_write system call to write a character to a specified file descriptor.
 *
 * @param c The character to write.
 * @param fd The file descriptor to write to.
 * @return int64_t Returns the 0 if the operation was successful, or -1 if an error occurred.
 */
int64_t libc_fputc ( char c, uint64_t fd )
{
	return sys_write ( fd, &c, 1 ) == -1 ? -1 : 0;
}


/**
 * @brief Writes formatted output to a specified file descriptor.
 *
 * This auxiliary function is used by libc_printf and libc_fprintf to write formatted output to a specified file descriptor.
 * It takes a variable argument list and a format string, which specifies how subsequent arguments are converted for output.
 *
 * @param fd The file descriptor to write to.
 * @param fmt The format string that specifies how subsequent arguments are converted for output.
 * @param argv A variable argument list.
 * @return int64_t Returns the number of characters written if the operation was successful, or -1 if an error occurred.
 */
#define BUFF_SIZE 64
static int64_t libc_vfprintf ( uint64_t fd, const char * fmt, va_list argv )
{
	uint64_t flag = 0;
	uint64_t written = 0;
	char buffer[BUFF_SIZE];


	for ( uint64_t i = 0; fmt[i] != '\0'; i++ ) {
		if ( fmt[i] == '%' && !flag ) {
			flag = 1;
			i++;
		}

		if ( !flag ) {
			libc_fputc ( fmt[i], fd );
			flag = 0;
			written++;
			continue;
		}
	

		switch ( fmt[i] ) {
			case 'c':
				libc_fputc ( va_arg ( argv, int ), fd );
				written++;
				break;
			case 'd':
				written += libc_vfprintf ( fd, libc_num_to_string ( va_arg ( argv, uint64_t ), 10 , buffer, BUFF_SIZE), argv );
				break;
			case 'x':
				written += libc_vfprintf ( fd, "0x", argv );
				written += libc_vfprintf ( fd, libc_num_to_string ( va_arg ( argv, uint64_t ), 16, buffer, BUFF_SIZE ), argv );
				break;
			case 's':
				written += libc_vfprintf ( fd, va_arg ( argv, char * ), argv );
				break;
			case '%':
				libc_fputc ( '%', fd );
				written++;
				break;
			default:
				return -1;
		}

		flag = 0;
	}

	return written;
}



// libc_fprintf and libc_printf were copied from this website. libc_vfprintf was implemented by us.
// http://www.firmcodes.com/write-libc_printf-function-c/
/**
 * @brief Writes formatted output to a specified file descriptor.
 *
 * @param fd The file descriptor to write to.
 * @param fmt The format string that specifies how subsequent arguments are converted for output.
 * @param ... Variable argument list.
 * @return int64_t Returns the number of characters written if the operation was successful, or -1 if an error occurred.
 */
int64_t libc_fprintf ( uint64_t fd, const char * fmt, ... )
{
	va_list argv;
	va_start ( argv, fmt );

	int64_t out = libc_vfprintf ( fd, fmt, argv );

	va_end ( argv );
	return out;
}



/**
 * @brief Writes formatted output to the standard output (STDOUT).
 *
 * @param fmt The format string that specifies how subsequent arguments are converted for output.
 * @param ... Variable argument list.
 * @return int64_t Returns the number of characters written if the operation was successful, or -1 if an error occurred.
 */
int64_t libc_printf ( const char * fmt, ... )
{
	va_list argv;
	va_start ( argv, fmt );

	int64_t out = libc_vfprintf ( STDOUT, fmt, argv );

	va_end ( argv );
	return out;
}



/**
 * @brief Reads a specified number of characters from the standard input.
 *
 * This function reads characters from the standard input until it encounters a newline.
 * If it has read the specified number of characters it will cease to write on the buffer unless it encounters a backspace.
 * The characters are stored in a buffer, which is then returned.
 *
 * @param buffer The buffer to store the characters.
 * @param n The maximum number of characters to read. (the last character will be a null terminator)
 * @return char* Returns a pointer to the buffer.
 */
char * libc_gets ( char * buffer, int n )
{
	int c;
	int i = 0;

	while ( ( c = libc_get_char() ) != '\n' ) {
		if ( c == '\b' && i > 0 ) {
			libc_put_char ( c );
			i--;
		}
		if ( c != '\b' && i < n - 1 ) {
			libc_put_char ( c );
			buffer[i++] = ( char ) c;
		}
	}
	libc_put_char ( '\n' );
	buffer[i] = '\0';

	return buffer;
}



/**
 * @brief Compares two strings.
 *
 * This function compares the two strings str1 and str2. It returns an integer less than, equal to, or greater than zero
 * if str1 is found, respectively, to be less than, to match, or be greater than str2.
 *
 * @param str1 The first string to be compared.
 * @param str2 The second string to be compared.
 * @return int Returns an integer less than, equal to, or greater than zero if str1 is found, respectively, to be less than, to match, or be greater than str2.
 */
int64_t libc_strcmp ( const char * str1, const char * str2 )
{
	while ( *str1 && ( *str1 == *str2 ) ) {
		str1++;
		str2++;
	}
	return * ( unsigned char * ) str1 - * ( unsigned char * ) str2;
}



/**
 * @brief Switches the system to video mode.
 *
 * @return int64_t Returns 0 if the mode was successfully set, or -1 if an error occurred.
 */
int64_t libc_enter_video_mode()
{
	return sys_set_mode ( VIDEO_MODE );
}



/**
 * @brief Switches the system to text mode.
 *
 * @return int64_t Returns 0 if the mode was successfully set, or -1 if an error occurred.
 */
int64_t libc_enter_text_mode()
{
	return sys_set_mode ( TEXT_MODE );
}



/**
 * @brief Draws a pixel at a specific position with a specific color.
 *
 * @param x The x-coordinate of the pixel.
 * @param y The y-coordinate of the pixel.
 * @param color The color of the pixel.
 * @return int64_t Returns 0 if the pixel was successfully drawn, or -1 if an error occurred.
 */
int64_t libc_draw_pixel ( uint64_t x, uint64_t y, color color )
{
	return sys_put_pixel ( x, y, &color );
}



/**
 * @brief Draws a rectangle at a specific position with a specific width, height, and color.
 *
 * @param x The x-coordinate of the top-left corner of the rectangle.
 * @param y The y-coordinate of the top-left corner of the rectangle.
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 * @param color The color of the rectangle.
 * @return int64_t Returns 0 if the rectangle was successfully drawn, or -1 if an error occurred.
 */
int64_t libc_draw_rectangle ( uint64_t x, uint64_t y, uint64_t width, uint64_t height, color color )
{
	return sys_put_rectangle ( x, y, width, height, &color );
}


/**
 * @brief Draws a letter at a specific position with a specific color.
 *
 * @param x The x-coordinate of the top-left corner of the letter.
 * @param y The y-coordinate of the top-left corner of the letter.
 * @param letter The letter to be drawn.
 * @param color The color of the letter.
 * @param fontSize The size of the font.
 * @return int64_t Returns 0 if the letter was successfully drawn, or -1 if an error occurred.
 */
int64_t libc_draw_letter ( uint64_t x, uint64_t y, char letter, color color, uint64_t font_size )
{
	return sys_draw_letter ( x, y, &letter, &color, font_size );
	// int64_t sys_draw_letter(uint64_t x, uint64_t y, char * letter, color * color, uint64_t fontSize)
}



/**
 * @brief Prints the current state of the CPU registers.
 *
 * This function retrieves the snapshot of the CPU registers (which is created by pressing the F1 key)
 *
 * @note The snapshot is only available if the F1 key has been pressed. If the F1 key has not been pressed, this function will print an error message.
 *
 * @return void
 */
void libc_print_register_snapshot()
{
	snapshot snap;
	if ( sys_get_register_snapshot ( &snap ) == -1 ) {
		libc_fprintf ( STDERR, "No register snapshot available. Press F1 to take a snapshot.\n" );
		return;
	}

	libc_puts ( "Register snapshot:\n" );
	libc_printf ( "rax: %x\n", snap.rax );
	libc_printf ( "rbx: %x\n", snap.rbx );
	libc_printf ( "rcx: %x\n", snap.rcx );
	libc_printf ( "rdx: %x\n", snap.rdx );
	libc_printf ( "rsi: %x\n", snap.rsi );
	libc_printf ( "rdi: %x\n", snap.rdi );
	libc_printf ( "rbp: %x\n", snap.rbp );
	libc_printf ( "rsp: %x\n", snap.rsp );
	libc_printf ( "r8:  %x\n", snap.r8 );
	libc_printf ( "r9:  %x\n", snap.r9 );
	libc_printf ( "r10: %x\n", snap.r10 );
	libc_printf ( "r11: %x\n", snap.r11 );
	libc_printf ( "r12: %x\n", snap.r12 );
	libc_printf ( "r13: %x\n", snap.r13 );
	libc_printf ( "r14: %x\n", snap.r14 );
	libc_printf ( "r15: %x\n", snap.r15 );
	libc_printf ( "rIP: %x\n", snap.rip );
}

//----------------------------- TP2 ---------------------------------- //


void * libc_malloc ( uint64_t size )
{
	return sys_malloc ( size );
}

void libc_free ( void * p )
{
	return sys_free ( p );
}

int64_t libc_create_process( main_function rip, uint64_t priority, char ** argv, uint64_t argc)
{
	return sys_create_process(rip, priority, argv, argc);
}
int64_t libc_get_time ( time_struct * time ){
	return sys_get_time ( time );
}

int64_t libc_get_pid()
{
	return sys_get_pid();
}

int64_t libc_kill ( pid_t pid )
{
	return sys_kill(pid);
}
int64_t libc_block ( pid_t pid )
{
	return sys_block(pid);
}
int64_t libc_nice(pid_t pid, uint64_t newPrio )
{
	return sys_nice(pid, newPrio);
}

int64_t libc_yield()
{
	return sys_yield();
}
int64_t libc_unblock ( pid_t pid )
{
	return sys_unblock(pid);
}

pid_t libc_wait ( pid_t pid, int64_t * ret){
	return sys_wait(pid, ret);
}





/*
void ps(){
	(process_info_list *) process_list = sys_ps();
	if(process_list == NULL){
		libc_printf("Error while getting process list\n");
		return;
	}
	libc_printf("PID\tPPID\tPriority\tBase Pointer\tStack Pointer\tStatus\n");
	for(int i = 0; i < process_list->amount_of_processes; i++){
		libc_printf("%d\t%d\t%d\t%d\t%d\t%d\n", process_list->processes[i].pid, process_list->processes[i].ppid, process_list->processes[i].priority, process_list->processes[i].base_pointer, process_list->processes[i].stack_pointer, process_list->processes[i].status);
	}
}
*/