#ifndef MEMORY_MANAGEMENT_H
#define MEMORY_MANAGEMENT_H

#include <stdint.h>

void my_mm_init(void *p, uint64_t s);
void *my_malloc(uint64_t size);
void my_free(void *p);



#endif