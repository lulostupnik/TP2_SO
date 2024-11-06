#ifndef _SHARED_LIBC_H
#define _SHARED_LIBC_H
#include <stdint.h>
#include <shared_defs.h>

void * shared_libc_memset ( void * destination, int32_t c, uint64_t length );
void * shared_libc_memcpy ( void * destination, const void * source, uint64_t length );
char ** shared_libc_copy_argv(pid_t pid, char ** argv, uint64_t argc, void *(*malloc_fun)(uint64_t), void (*free_fun)(void *));
char * shared_libc_new_str_copy(char * string, void *(*malloc_fun)(uint64_t));


#endif