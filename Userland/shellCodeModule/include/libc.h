#ifndef TPE_ARQUI_LIBC_H
#define TPE_ARQUI_LIBC_H

#include <stdint.h>
#include <syscallFunctions.h>
#include <stdarg.h>

#define STDIN 0
#define STDOUT 0
#define STDERR 1
#define TEXT_MODE 0
#define VIDEO_MODE 1
#define NULL 0

typedef enum {
	LEFT_CONTROL = 256,         // NOTAR: El valor con el que empieza tiene que ser mas grande al ultimo de la tabla ASCII
	LEFT_SHIFT,
	RIGHT_SHIFT,
	KEYPAD_STAR_PRTSC,
	LEFT_ALT,
	CAPS_LOCK,
	F1,
	F2,
	F3,
	F4,
	F5,
	F6,
	F7,
	F8,
	F9,
	F10,
	NUM_LOCK,
	SCROLL_LOCK,
	HOME_KEYPAD,
	UP_ARROW_KEYPAD,
	PG_UP_KEYPAD,
	LEFT_ARROW_KEYPAD,
	RIGHT_ARROW_KEYPAD,
	PLUS_KEYPAD,
	END_KEYPAD,
	DOWN_ARROW_KEYPAD,
	PG_DN_KEYPAD,
	INS_KEYPAD,
	DEL_KEYPAD,
	SYS_REQ_AT,
	NONE1, // no hay tecla de scanCode 0x55
	NONE2, // no hay tecla de scan code 0x56
	F11,
	F12
} specialCodes;





/**
 * @brief Reads a character from the standard input.
 *
 * This function uses the read system call to read a character from the standard input (STDIN) (the keyboard buffer)
 * It blocks until a character is available.
 *
 * @return int64_t Returns the value of the read character.
 */
char getChar();



/**
 * @brief Writes a character to the standard output.
 *
 * This function uses the sys_write system call to write a character to the standard output (STDOUT)
 * (The character is written to the screen)
 *
 * @param c The character to write.
 */
void put_char ( char c );



/**
 * @brief Generates a beep sound.
 *
 * This function uses the sys_beep system call to generate a beep sound.
 * The beep sound is produced by the system speaker.
 *
 * @param frequency The frequency of the beep sound in hertz.
 * @param duration The duration of the beep sound in milliseconds.
 * @return int64_t Returns 0 if the beep was successfully generated, or -1 if an error occurred.
 */
int64_t beep ( uint64_t frequency, uint64_t duration );



/**
 * @brief Clears the system screen.
 *
 * This function uses the sys_clear_screen system call to clear the system screen.
 * The screen is cleared to black.
 *
 * @return int64_t Returns 0 if the screen was successfully cleared, or -1 if an error occurred.
 */
int64_t clear_screen();



/**
 * @brief Sets the font size of the system console.
 *
 * This function uses the sys_set_font_size system call to set the font size of the system console.
 * The font size is set to the specified size.
 *
 * @param size The desired font size.
 * @return int64_t Returns 0 if the font size was successfully set, or -1 if an error occurred.
 */
int64_t setFontSize ( uint64_t size );



/**
 * @brief Calculates the length of a string.
 *
 * This function calculates the length of the null terminated string pointed to by `str`, excluding the terminating null byte ('\0').
 *
 * @param str The string whose length is to be calculated.
 * @return size_t Returns the number of characters in the string pointed to by `str`.
 */
uint64_t strlen ( const char * str );



/**
 * @brief Converts a number to a string representation in a specified base.
 *
 * This function converts a number to its string representation in a specified base.
 * It uses a static buffer to hold the result, so the returned string should be used or copied before the next call to `numToString`.
 *
 * @param num The number to be converted.
 * @param base The base to use for the conversion. This should be between 2 and 16 inclusive.
 * @return char* Returns a pointer to the string representation of the number. This string is null-terminated.
 */
char * numToString ( uint64_t num, uint64_t base );



/**
 * @brief Writes a string to the standard output.
 *
 * This function uses the sys_write system call to write a string to the standard output (STDOUT).
 * The string is written to the screen. A newline character is appended at the end of the string.
 *
 * @param str The string to write.
 * @return int64_t Returns the number of characters written if the operation was successful, or -1 if an error occurred.
 */
int64_t puts ( const char * str );



/**
 * @brief Writes a character to a file descriptor.
 *
 * This function uses the sys_write system call to write a character to a specified file descriptor.
 *
 * @param c The character to write.
 * @param fd The file descriptor to write to.
 * @return int64_t Returns the 0 if the operation was successful, or -1 if an error occurred.
 */
int64_t fputc ( char c, uint64_t fd );



/**
 * @brief Writes formatted output to a specified file descriptor.
 *
 * @param fd The file descriptor to write to.
 * @param fmt The format string that specifies how subsequent arguments are converted for output.
 * @param ... Variable argument list.
 * @return int64_t Returns the number of characters written if the operation was successful, or -1 if an error occurred.
 */
int64_t fprintf ( uint64_t fd, const char * fmt, ... );



/**
 * @brief Writes formatted output to the standard output (STDOUT).
 *
 * @param fmt The format string that specifies how subsequent arguments are converted for output.
 * @param ... Variable argument list.
 * @return int64_t Returns the number of characters written if the operation was successful, or -1 if an error occurred.
 */
int64_t printf ( const char * fmt, ... );



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
char * gets ( char * buffer, int n );



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
int64_t strcmp ( const char * str1, const char * str2 );



/**
 * @brief Prints the current state of the CPU registers. Use F1 to create snapshot. Only last snapshot will be shown.
 *
 * This function prints the snapshot of the CPU registers (which is created by pressing the F1 key) or an error message if no snapshot is available.
 *
 * @note The snapshot is only available if the F1 key has been pressed. If the F1 key has not been pressed, this function will print an error message.
 *
 * @return void
 */
void print_register_snapshot();



/**
 * @brief Draws a pixel at a specific position with a specific color.
 *
 * @param x The x-coordinate of the pixel.
 * @param y The y-coordinate of the pixel.
 * @param color The color of the pixel.
 * @return int64_t Returns 0 if the pixel was successfully drawn, or -1 if an error occurred.
 */
int64_t draw_pixel ( uint64_t x, uint64_t y, color color );



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
int64_t draw_rectangle ( uint64_t x, uint64_t y, uint64_t width, uint64_t height, color color );



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
int64_t draw_letter ( uint64_t x, uint64_t y, char letter, color color, uint64_t font_size );



/**
 * @brief Switches the system to text mode.
 *
 * @return int64_t Returns 0 if the mode was successfully set, or -1 if an error occurred.
 */
int64_t enter_text_mode();



/**
 * @brief Switches the system to video mode.
 *
 * @return int64_t Returns 0 if the mode was successfully set, or -1 if an error occurred.
 */
int64_t enter_video_mode();


void * memset ( void * destination, int32_t c, uint64_t length );

void * my_malloc ( uint64_t size );

void my_free ( void * ptr );

int64_t my_create_process( main_function rip, uint64_t priority, char ** argv, uint64_t argc);

int64_t my_get_pid();

int64_t my_nice ( int64_t pid, uint64_t newPrio );

int64_t my_yield();

int64_t my_kill ( int64_t pid );

int64_t my_block ( int64_t pid );

int64_t my_unblock ( int64_t pid );
#endif
