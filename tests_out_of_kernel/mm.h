#ifndef MM_H
#define MM_H

#include <stdlib.h>
#include "test_mm.h"
#include "syscall.h"
#include "test_util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 1024
#define BLOCK_COUNT 1024
#define MEM_SIZE_STR "1048576"

void my_mm_init ( void *p, int s );

void *my_malloc ( int size );

void my_free ( void *p );

#endif