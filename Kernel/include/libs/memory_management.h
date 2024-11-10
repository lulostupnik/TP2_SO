#ifndef MEMORY_MANAGEMENT_H
#define MEMORY_MANAGEMENT_H

#include "syscall.h"
#include <stdint.h>
#include <string.h>
#include <stdint.h>
#include <shared_defs.h>

void my_mm_init ( void * p );
void * my_malloc ( uint64_t size );
void my_free ( void * p );
int64_t my_mem_info( memory_info * info );


#define BLOCK_SIZE 0x1000                       // 4k
#define HEAP_SIZE 0x1000000                     // 16MB
#define BLOCK_COUNT (HEAP_SIZE / BLOCK_SIZE)

#endif