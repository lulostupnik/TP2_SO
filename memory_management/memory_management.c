// No alinear
// No gestionar fragmentación
// No pide tamaño de bloque

// Si liberar memoria
#include <stdlib.h>
// #include "test_mm.h"
#include "syscall.h"
#include "test_util.h"
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

void *my_malloc()
{
    if (current < BLOCK_COUNT)
    {
        return free_ptrs[current++];
    }
    else
    {
        return NULL;
    }
}

void my_free(void *p)
{
    free_ptrs[--current] = p;
}

int main()
{
    mm_rq mm_rqs[MAX_BLOCKS];
    uint8_t rq;
    uint32_t total;
    uint64_t max_memory = BLOCK_SIZE * BLOCK_COUNT;

    char mem[BLOCK_COUNT * BLOCK_SIZE];

    my_mm_init((void *)mem, BLOCK_COUNT * BLOCK_SIZE);

    /*
    for (int i = 0; i < BLOCK_COUNT; i++)
    {
        printf("free_ptrs[%d]: %p\n", i, free_ptrs[i]);
    }
    return 0;
    */

    rq = 0;
    total = 0;

    // Request as many blocks as we can

    for (int j = 0; j < 2; j++)
    {
        rq = 0;

        while (rq < BLOCK_COUNT)
        {
            printf("rq: %d\n", rq);
            mm_rqs[rq].size = BLOCK_SIZE;
            mm_rqs[rq].address = my_malloc(/*mm_rqs[rq].size*/);

            if (mm_rqs[rq].address)
            {
                total += mm_rqs[rq].size;
                rq++;
            }
        }
        // Set
        uint32_t i;
        for (i = 0; i < rq; i++)
            if (mm_rqs[i].address)
                memset(mm_rqs[i].address, i, mm_rqs[i].size);

        // Check
        for (i = 0; i < rq; i++)
            if (mm_rqs[i].address)
                if (!memcheck(mm_rqs[i].address, i, mm_rqs[i].size))
                {
                    printf("test_mm ERROR\n");
                    return -1;
                }

        // Free
        for (i = 0; i < rq; i++)
            if (mm_rqs[i].address)
                my_free(mm_rqs[i].address);
    }
}
