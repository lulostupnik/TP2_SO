#ifndef _PIPE_H
#define _PIPE_H

#include <stdint.h>
#include <semaphores.h>
#include <shared_defs.h>

#define PIPE_BUFFER_SIZE 2
#define AMOUNT_OF_PIPES 100


void pipe_init();
int64_t pipe_open(int64_t id, pipe_mode_t mode);
int64_t pipe_read(int64_t id, uint16_t * buffer, uint64_t amount);
int64_t pipe_write(int64_t id, uint16_t * buffer, uint64_t amount);
int64_t pipe_close(int64_t id);
int64_t pipe_open_free(pipe_mode_t mode);
pid_t pipe_get_pid(int64_t id, pipe_mode_t mode);
pid_t pipe_reserve();
int64_t pipe_open_pid(int64_t id, pipe_mode_t mode, pid_t pid);

#endif