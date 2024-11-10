#ifndef MEMORY_MANAGEMENT_H
#define MEMORY_MANAGEMENT_H

#include "syscall.h"
#include <stdint.h>
#include <string.h>
#include <stdint.h>
#include <shared_defs.h>


typedef struct memory_manager_cdt * memory_manager_adt;

memory_manager_adt my_mm_init ( void * p );
void * my_malloc ( uint64_t size , struct memory_manager_cdt * mem);
void my_free ( void * p , memory_manager_adt adt);
int64_t my_mem_info( memory_info * info , memory_manager_adt adt);



#define BLOCK_SIZE 0x1000                       // 4k
#define HEAP_SIZE 0x1000000                     // 16MB

#endif