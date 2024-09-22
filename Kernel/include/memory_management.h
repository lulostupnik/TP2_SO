#ifndef MEMORY_MANAGEMENT_H
#define MEMORY_MANAGEMENT_H

#include <stdint.h>

void my_mm_init(void *p, uint64_t s);
void *my_malloc(uint64_t size);
void my_free(void *p);

#define MAX_BLOCKS 20
#define BLOCK_SIZE 0x400
#define HEAP_SIZE 0x100000
#define BLOCK_COUNT (HEAP_SIZE / BLOCK_SIZE)

#endif