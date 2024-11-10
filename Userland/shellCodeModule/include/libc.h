#ifndef TPE_ARQUI_LIBC_H
#define TPE_ARQUI_LIBC_H

#include <stdint.h>
#include <stdarg.h>
#include <driversInformation.h>
#include <shared_defs.h>
#include <syscalls.h>
#include <stddef.h>
#include <shared_libc.h>

char libc_get_char();
void libc_put_char ( char c );
int64_t libc_beep ( uint64_t frequency, uint64_t duration );
int64_t libc_clear_screen();
int64_t libc_set_font_size ( uint64_t size );
char * libc_num_to_string(uint64_t num, uint64_t base, char *buffer, size_t buffer_size);
int64_t libc_puts (  char * str );
int64_t libc_fputc ( char c, uint64_t fd );
int64_t libc_fprintf ( uint64_t fd,  char * fmt, ... );
int64_t libc_printf (  char * fmt, ... );
char * libc_gets ( char * buffer, int n );
int64_t libc_strcmp (  char * str1,  char * str2 );
int64_t libc_strnocasecmp( char *str1,  char *str2);
void libc_print_register_snapshot();
int64_t libc_draw_pixel ( uint64_t x, uint64_t y, color color );
int64_t libc_draw_rectangle ( uint64_t x, uint64_t y, uint64_t width, uint64_t height, color color );
int64_t libc_draw_letter ( uint64_t x, uint64_t y, char letter, color color, uint64_t font_size );
int64_t libc_enter_text_mode();
int64_t libc_enter_video_mode();
int64_t libc_get_time ( time_struct * time );
void * libc_malloc ( uint64_t size );
void libc_free ( void * ptr );
pid_t libc_create_process( main_function rip, uint64_t priority, char ** argv, uint64_t argc, fd_t fds[]);
pid_t libc_get_pid();
int64_t libc_nice ( pid_t pid, uint64_t newPrio );
int64_t libc_yield();
int64_t libc_kill ( pid_t pid );
int64_t libc_block ( pid_t pid );
int64_t libc_unblock ( pid_t pid );
pid_t libc_wait( pid_t pid, int64_t * ret);
int64_t libc_sem_open ( int64_t sem_id, int64_t value );
int64_t libc_sem_open_get_id( int64_t value );
int64_t libc_sem_wait ( int64_t sem_id );
int64_t libc_sem_post ( int64_t sem_id );
int64_t libc_sem_close ( int64_t sem_id );
int64_t libc_ticks_sleep ( uint64_t ticks );
int8_t libc_get_status(pid_t pid);
int64_t libc_pipe_open(int64_t id, pipe_mode_t mode);
int64_t libc_pipe_open_free(pipe_mode_t mode);
int64_t libc_pipe_read(int64_t id, uint8_t * buffer, uint64_t amount);
int64_t libc_pipe_write(int64_t id, uint8_t * buffer, uint64_t amount);
int64_t libc_pipe_close(int64_t id);
int64_t libc_pipe_reserve();
int64_t libc_satoi ( char * str, int64_t * flag );
int64_t libc_get_my_fds(fd_t fds[CANT_FDS]);

#endif
