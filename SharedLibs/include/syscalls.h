#ifndef _SYSCALLS_H
#define _SYSCALLS_H

#include <stdint.h>
#include <shared_defs.h>
#include <driversInformation.h>

#define NOT_VALID_SYS_ID -1
#define NUM_SYSCALLS 38 

int64_t sys_read (uint8_t * buffer, uint64_t amount);
int64_t sys_write (uint64_t fd, uint8_t * buffer, uint64_t amount);
int64_t sys_get_register_snapshot (snapshot * snapshot);
int64_t sys_beep (uint32_t frequency, uint32_t duration);
int64_t sys_set_font_size (uint64_t size);
int64_t sys_clear_screen (void);
int64_t sys_put_pixel (uint64_t x, uint64_t y, color * color);
int64_t sys_put_rectangle (uint64_t x, uint64_t y, uint64_t width, uint64_t height, color * color);
int64_t sys_draw_letter (uint64_t x, uint64_t y, char * letter, color * color, uint64_t fontSize);
int64_t sys_set_mode (uint64_t mode);
int64_t sys_get_screen_information (screen_information * screen_information);
int64_t sys_ticks_sleep (uint64_t ticks);
int64_t sys_get_time (time_struct * time);
void * sys_malloc (uint64_t size);
void sys_free (void * p);
int64_t sys_get_pid();
int64_t sys_create_process (main_function rip, priority_t priority, char ** argv, uint64_t argc, fd_t fds[]);
int64_t sys_nice (pid_t pid, uint64_t newPrio);
int64_t sys_kill (pid_t pid);
int64_t sys_block (pid_t pid);
int64_t sys_unblock (pid_t pid);
int64_t sys_yield();
int64_t sys_wait (int64_t pid, int64_t * ret);
int64_t sys_sem_open (int64_t sem_id, int value);
int64_t sys_sem_wait (int64_t sem_id);
int64_t sys_sem_post (int64_t sem_id);
int64_t sys_sem_close (int64_t sem_id);
int64_t sys_sem_open_get_id(int value);
process_info_list * sys_ps();
int8_t sys_get_status(pid_t pid);
void sys_free_ps(process_info_list * ps);
int64_t sys_pipe_open(int64_t id, pipe_mode_t mode);
int64_t sys_pipe_open_free(pipe_mode_t mode);
int64_t sys_pipe_read(int64_t id, uint8_t * buffer, uint64_t amount);
int64_t sys_pipe_write(int64_t id, uint8_t * buffer, uint64_t amount);
int64_t sys_pipe_close(int64_t id);
int64_t sys_pipe_reserve();
int64_t sys_get_my_fds(fd_t fds[CANT_FDS]);

#endif

