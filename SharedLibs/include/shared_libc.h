#ifndef _SHARED_LIBC_H
#define _SHARED_LIBC_H
#include <stdint.h>
#include <shared_defs.h>

void * shared_libc_memset ( void * destination, int32_t c, uint64_t length );
void * shared_libc_memcpy ( void * destination, const void * source, uint64_t length );
uint64_t shared_libc_strlen ( const char * str );

#endif