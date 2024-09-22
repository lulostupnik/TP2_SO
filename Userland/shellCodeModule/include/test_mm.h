
#ifndef TEST_MM_H
#define TEST_MM_H

#include <stdint.h>
#include "test_util.h"

#define MAX_BLOCKS 256

void * my_malloc(uint64_t size);
void my_free(void *ptr);
//void my_mm_init(void *p, int s);

#endif
