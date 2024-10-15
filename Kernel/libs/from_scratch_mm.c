// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <memory_management.h>

typedef struct MM_rq {
	void *address;
	uint32_t size;
} mm_rq;

void *start;
int size;
int current;
void *free_ptrs[BLOCK_COUNT]; 

void my_mm_init ( void *p, uint64_t s )
{
	start = p;
	size = s;

	for ( int i = 0; i < BLOCK_COUNT; i++ ) {
		free_ptrs[i] = start + i * BLOCK_SIZE;
	}

	current = 0;
}

void *my_malloc ( uint64_t size )
{
	if ( size > BLOCK_SIZE || current >= BLOCK_COUNT ) {
		return NULL;
	}
	 
	return free_ptrs[current++];
}

void my_free ( void *p )
{
	free_ptrs[--current] = p;
}

