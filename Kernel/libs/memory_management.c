// No alinear
// No gestionar fragmentación
// No pide tamaño de bloque

// Si liberar memoria

// #include "test_mm.h"

#include <stdlib.h>
#include "syscall.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BLOCKS 20
#define BLOCK_SIZE 10
#define BLOCK_COUNT 10

typedef struct MM_rq
{
    void *address;
    uint32_t size;
} mm_rq;

void *start;
int size;
int current;
void *free_ptrs[BLOCK_COUNT]; // [10, 20, 30, 40, 50] --> [20, 30, 40, 50] --> [30, 40, 50]

void my_mm_init(void *p, int s)
{
    start = p;
    size = s;

    for (int i = 0; i < BLOCK_COUNT; i++)
    {
        free_ptrs[i] = start + i * BLOCK_SIZE;
    }

    current = 0;
}

void *my_malloc(int size)
{
    if (size > BLOCK_SIZE || current >= BLOCK_COUNT)
    {
        return NULL;
    }
    return free_ptrs[current++];
}

void my_free(void *p)
{
    free_ptrs[--current] = p;
}

