
#ifndef _SYSCALLS_H
#define _SYSCALLS_H

#include <stdint.h>
#include <shared_defs.h>
#include <driversInformation.h>
//#include <registerSaver.h>


#define NOT_VALID_SYS_ID -1



/**
 * @brief Reads a specified number of bytes from a file descriptor into a buffer.
 *
 * This system call reads 'amount' number of characters from the file descriptor 'fd' into the buffer pointed to by 'buffer'.
 * It is typically used to read input from the standard input (STDIN), but can be used with any valid file descriptor.
 *
 * @param fd The file descriptor from which to read. This is typically STDIN.
 * @param buffer A pointer to a buffer where the read bytes will be stored.
 * @param amount The number of characters to read from the file descriptor.
 * @return int64_t Returns the number of characters actually read. This may be less than 'amount' if fewer characters are available.
 *                 Returns -1 if an error occurred (for example, if 'fd' is not a valid file descriptor).
 */
int64_t sys_read ( uint64_t fd, uint16_t * buffer, uint64_t amount );



/**
 * @brief Writes a specified number of characters from a buffer to a file descriptor.
 *
 * This system call writes 'amount' number of characters from the buffer pointed to by 'buffer' to the file descriptor 'fd'.
 * It is typically used to write output to the standard output (STDOUT) or standard error (STDERR), but can be used with any valid file descriptor. (For now it only allows STDOUT and writes directly on the screen)
 *
 * @param fd The file descriptor to which to write. This is typically STDOUT or STDERR.
 * @param buffer A pointer to a buffer containing the characters to write.
 * @param amount The number of characters to write to the file descriptor.
 * @return int64_t Returns the number of characters actually written. This may be less than 'amount' if there was an error writing to the file descriptor (or '\0' is encountered?)
 *                 Returns -1 if an error occurred (for example, if not in text mode, if 'fd' is not a valid file descriptor, ...).
 */
int64_t sys_write ( uint64_t fd, const char * buffer, uint64_t amount );



/**
 * @brief Retrieves the saved state of the registers.
 *
 * If the registers have been previously saved, this function writes the saved state into the provided TRegs structure.
 * If no registers have been saved, the function does not modify the provided structure.
 *
 * @param registers Pointer to a RegisterSet structure where the saved state of the registers will be written.
 * @return int64_t Returns 0 if the registers were previously saved and their state has been written into the provided structure.
 *                 Returns -1 if no registers have been saved, in which case the provided structure is not modified.
 */
int64_t sys_get_register_snapshot ( snapshot * snapshot );


/**
 * @brief Generates a libc_beep sound using the system speaker.
 *
 * This system call interfaces with the system's hardware to generate a libc_beep sound.
 * The frequency and duration of the libc_beep are specified by the parameters.
 *
 * @param frequency The frequency of the libc_beep in Hertz. This value must be between 20 and 20000.
 * @param duration The duration of the libc_beep in milliseconds.
 * @return int64_t Returns 0 on success, or -1 if an error occurred (for example, if the frequency or duration is out of range).
 */
int64_t sys_beep ( uint32_t frequency, uint32_t duration );



/**
 * @brief Sets the system font size.
 *
 * @param size The new font size.
 * @return int64_t Returns 0 if the font size was successfully set, or -1 if an error occurred
 *             (for example, if not in text mode or if 'size' is not a valid font size).
 */
int64_t sys_set_font_size ( uint64_t size );



/**
 * @brief Clears the system screen.
 *
 * This system call interfaces with the system's hardware to clear the screen. (black)
 *
 * @return int64_t Returns 0 if the screen was successfully cleared, or -1 if an error occurred.
 */
int64_t sys_clear_screen ( void );




/**
 * @brief Draws a pixel on the system screen at a specified position with a specified color.
 *
 * This system call interfaces with the system's hardware to draw a pixel on the screen.
 *
 * @param x The x-coordinate of the pixel.
 * @param y The y-coordinate of the pixel.
 * @param color A pointer to structure representing the RGB color values of the pixel.
 * @return int64_t Returns 0 if the pixel was successfully drawn, or -1 if an error occurred
 *             (for example, if 'x' or 'y' is out of the screen bounds, or 'color' is not a valid color).
 */
int64_t sys_put_pixel ( uint64_t x, uint64_t y, color * color );



/**
 * @brief Draws a rectangle on the system screen at a specified position with a specified color, width, and height.
 *
 * This system call interfaces with the system's hardware to draw a rectangle on the screen.
 *
 * @param x The x-coordinate of the top-left corner of the rectangle.
 * @param y The y-coordinate of the top-left corner of the rectangle.
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 * @param color A pointer to structure representing the RGB color values of the rectangle.
 * @return int64_t Returns 0 if the rectangle was successfully drawn, or -1 if an error occurred
 *             (for example, if 'x', 'y', 'width', or 'height' is out of the screen bounds, or 'color' is not a valid color).
 */
int64_t sys_put_rectangle ( uint64_t x, uint64_t y, uint64_t width, uint64_t height, color * color );



/**
 * @brief Draws a letter on the system screen at a specified position with a specified color.
 *
 * This system call interfaces with the system's hardware to draw a letter on the screen.
 *
 * @param x The x-coordinate of the top-left corner of the letter.
 * @param y The y-coordinate of the top-left corner of the letter.
 * @param letter The letter to be drawn.
 * @param color A structure representing the RGB color values of the letter.
 * @param fontSize The size of the font to be used.
 * @return int Returns 0 if the letter was successfully drawn, or -1 if an error occurred
 *             (for example, if not in video mode, 'x' or 'y' is out of the screen bounds, 'letter' is not a valid character, or 'color' is not a valid color).
 */
int64_t sys_draw_letter ( uint64_t x, uint64_t y, char * letter, color * color, uint64_t fontSize );



/**
 * @brief Sets the system to a specified mode.
 *
 * @param mode The mode to which the system should be set. This should be either TEXT_MODE or VIDEO_MODE.
 * @return int64_t Returns 0 if the system was successfully set to the specified mode, -1 if an error occurred (for example if the mode is invalid).
 */
int64_t sys_set_mode ( uint64_t mode );



/**
 * @brief Gets the dimensions of the system screen.
 *
 * The dimensions are written into the 'screen_size' parameter.
 *
 * @param screen_information A pointer to a screen_information structure that will be filled with the screen's dimensions.
 * @return int64_t Returns 0 if the dimensions were successfully retrieved, or -1 if an error occurred.
 */
int64_t sys_get_screen_information ( screen_information * screen_information );



/**
 * @brief Pauses the execution of the current thread for a specified number of ticks. (each tick has a duration of 55ms)
 *
 * @param ticks The number of ticks to sleep. A tick is a unit of time defined by the system clock. The actual duration of a tick can vary depending on the system's hardware and configuration.
 * @return int64_t Returns 0 if the sleep was successfully initiated, or -1 if an error occurred.
 */
int64_t sys_nano_sleep ( uint32_t ticks );



/**
 * @brief Retrieves the current system time.
 *
 * This system call interfaces with the system's hardware clock to retrieve the current system time.
 * The time is written into the 'time' parameter, which is a pointer to a time_struct structure.
 *
 * @param time A pointer to a time_struct structure that will be filled with the current system time.
 * @return int64_t Returns 0 if the time was successfully retrieved, or -1 if an error occurred.
 */
int64_t sys_get_time ( time_struct * time );


//FOR TP2_SO


void * sys_malloc ( uint64_t size );
void sys_free ( void * p );
int64_t sys_get_pid();
int64_t sys_create_process ( main_function rip, priority_t priority, char ** argv, uint64_t argc );
int64_t sys_nice ( pid_t pid, uint64_t newPrio );
int64_t sys_kill ( pid_t pid );
int64_t sys_block ( pid_t pid );
int64_t sys_unblock ( pid_t pid );
int64_t sys_yield();
int64_t sys_wait ( int64_t pid, int64_t * ret );


//int64_t sys_sem_open ( char *sem_id, uint64_t initialValue );
//int64_t sys_sem_wait ( char *sem_id );
//int64_t sys_sem_post ( char *sem_id );
//int64_t sys_sem_close ( char *sem_id );

#endif



