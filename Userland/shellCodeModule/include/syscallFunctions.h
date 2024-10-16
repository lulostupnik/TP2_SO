#ifndef SYSCALLFUNCTIONS_H
#define SYSCALLFUNCTIONS_H
#include <stdint.h>


/**
 * Structure representing all the register snapshot
 */
typedef struct {
	uint64_t rax;
	uint64_t rbx;
	uint64_t rcx;
	uint64_t rdx;
	uint64_t rsi;
	uint64_t rdi;
	uint64_t rbp;
	uint64_t rsp;
	uint64_t r8;
	uint64_t r9;
	uint64_t r10;
	uint64_t r11;
	uint64_t r12;
	uint64_t r13;
	uint64_t r14;
	uint64_t r15;
	uint64_t rip;
} snapshot;

/**
 * Structure representing a color
 */
typedef struct {
	uint8_t r; // Red component
	uint8_t g; // Green component
	uint8_t b; // Blue component
} color;


/**
 * Structure representing the dimensions of the screen.
 */
typedef struct {
	int64_t width;
	int64_t height;
} screen_information;

/**
 * Structure representing current time
 */
typedef struct time {
	uint8_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minutes;
	uint8_t seconds;
} time_struct;

typedef int (*main_function)(char ** argv, uint64_t argc);


int64_t sys_read ( uint64_t fd, uint16_t * buffer, uint64_t amount );


int64_t sys_write ( uint64_t fd, const char * buffer, uint64_t amount );


int64_t sys_get_register_snapshot ( snapshot * snapshot );


int64_t sys_beep ( uint32_t frequency, uint32_t duration );


int64_t sys_set_font_size ( uint64_t size );


int64_t sys_clear_screen ( void );


int64_t sys_put_pixel ( uint64_t x, uint64_t y, color * color );


int64_t sys_put_rectangle ( uint64_t x, uint64_t y, uint64_t width, uint64_t height, color * color );


int64_t sys_draw_letter ( uint64_t x, uint64_t y, char * letter, color * color, uint64_t fontSize );


int64_t sys_set_mode ( uint64_t mode );


int64_t sys_get_screen_information ( screen_information * screen_information );


int64_t sys_nano_sleep ( uint32_t ticks );


int64_t sys_get_time ( time_struct * time );

void * sys_malloc ( uint64_t size );

void sys_free ( void * p );

int64_t sys_create_process ( main_function rip, uint64_t priority, char ** argv, uint64_t argc );

int64_t sys_get_pid();

int64_t sys_nice ( int64_t pid, uint64_t newPrio );

int64_t sys_yield();

int64_t sys_kill ( int64_t pid );

int64_t sys_block ( int64_t pid );

int64_t sys_unblock ( int64_t pid );
//int64_t sys_sem_open ( char *sem_id, uint64_t initialValue );
//int64_t sys_sem_wait ( char *sem_id );
//int64_t sys_sem_post ( char *sem_id );
//int64_t sys_sem_close ( char *sem_id );

//int64_t sys_wait ( int64_t pid );

#endif