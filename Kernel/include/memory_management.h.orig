#ifndef MEMORY_MANAGEMENT_H
#define MEMORY_MANAGEMENT_H

#include "syscall.h"
#include <stdint.h>
#include <string.h>
#include <stdint.h>

void my_mm_init(void *p, uint64_t s);
void *my_malloc(uint64_t size);
void my_free(void *p);


#define BLOCK_SIZE 0x400                       // 1024 bytes
#define HEAP_SIZE 0x100000                     // 1MB
#define BLOCK_COUNT (HEAP_SIZE / BLOCK_SIZE)   // 1024 

#endif