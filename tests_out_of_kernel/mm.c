#include "mm.h"

void *start;
int size;
int current;
void *free_ptrs[BLOCK_COUNT];

void my_mm_init ( void *p, int s )
{
	start = p;
	size = s;

	for ( int i = 0; i < BLOCK_COUNT; i++ ) {
		free_ptrs[i] = start + i * BLOCK_SIZE;
	}
	current = 0;
}

void *my_malloc ( int size )
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

int main()
{
	char mem[BLOCK_COUNT * BLOCK_SIZE];
	my_mm_init ( ( void * ) mem, BLOCK_COUNT * BLOCK_SIZE );
	printf ( "Corriendo el test...\nCortar con Ctrl+C\n" );
	test_mm ( 1, ( char *[] ) {
		MEM_SIZE_STR
	} );

	return 0;
}
