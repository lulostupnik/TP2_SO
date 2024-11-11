// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#ifdef BUDDY
#include <memory_management.h>

#define GET_SIBLING(i) (((i) % 2) ? ((i) + 1) : ((i) - 1))
#define GET_PARENT(i) (((i) - 1) / 2)

#define MAX_MEM_SIZE HEAP_SIZE
#define MIN_BLOCK_SIZE BLOCK_SIZE


#define TREE_BIT_MAP_SIZE (((MAX_MEM_SIZE / MIN_BLOCK_SIZE)) * 2)


static int next_power_of_2 ( int n );
static int get_index_level ( int index );
static int get_size_level ( int size );
static int get_block_from_index ( int index );
static void my_free_idx ( int index, int * flag, int n, memory_manager_adt mem );
static void *my_alloc ( int index, int level, memory_manager_adt mem );


typedef struct {
	void *start;
	char tree_bitmap[TREE_BIT_MAP_SIZE];
	uint64_t free_mem;
} memory_manager_cdt;



int64_t my_mem_info ( memory_info * info, memory_manager_adt mem )
{
	memory_manager_cdt * aux = ( memory_manager_cdt * ) mem;
	if ( info == NULL || aux == NULL ) {
		return -1;
	}
	info->total_size = MAX_MEM_SIZE;
	info->free = aux->free_mem;
	return 0;
}

memory_manager_adt my_mm_init ( void *p )
{
	memory_manager_cdt * aux = ( memory_manager_cdt * ) p;

	aux->start = ( void * ) ( ( char * ) p );

	for ( int i = 0; i < TREE_BIT_MAP_SIZE; i++ ) {
		aux->tree_bitmap[i] = 0;
	}
	aux->free_mem = MAX_MEM_SIZE;
	memory_manager_adt ans = my_malloc ( sizeof ( *aux ), ( memory_manager_adt ) aux );
	return ans;
}



void * my_malloc ( uint64_t size,  memory_manager_adt mem )
{
	memory_manager_cdt * aux = ( memory_manager_cdt * ) mem;
	if( aux == NULL ){
		return NULL;
	}
	int npo2 = next_power_of_2 ( size );
	int real_size = npo2 >= MIN_BLOCK_SIZE ? npo2 : MIN_BLOCK_SIZE;
	int level = get_size_level ( real_size );

	void * ptr = my_alloc ( 0, level, mem );
	if ( ptr != NULL ) {
		aux->free_mem -= real_size;
	}

	return ptr;
}

void my_free ( void * p, memory_manager_adt mem )
{
	memory_manager_cdt * aux = ( memory_manager_cdt * ) mem;
	if( aux == NULL ){
		return;
	}
	if ( ( p - aux->start ) % MIN_BLOCK_SIZE != 0 ) {
		return;
	}

	int index = ( ( p - aux->start ) / MIN_BLOCK_SIZE ) + MAX_MEM_SIZE / MIN_BLOCK_SIZE - 1; // we aux->start with the block of maximum granularity
	int flag = 0;
	int n = 1;
	my_free_idx ( index, &flag, n, mem );
}

static void *my_alloc ( int index, int level, memory_manager_adt mem )
{
	memory_manager_cdt * aux = ( memory_manager_cdt * ) mem;
	if ( aux == NULL ) {
		return NULL;
	}
	if ( level == 0 ) {
		if ( aux->tree_bitmap[index] ) {
			return NULL;
		}
		aux->tree_bitmap[index] = 1;
		return aux->start + get_block_from_index ( index ) * MIN_BLOCK_SIZE;
	}
	if ( aux->tree_bitmap[index] && !aux->tree_bitmap[2 * index + 1] && !aux->tree_bitmap[index * 2 + 2] ) {
		return NULL;
	}
	// This is because if the block is occupied, but its children are free, it means the block is not divided but rather fully occupied

	void *ptr = my_alloc ( index * 2 + 1, level - 1, mem );
	if ( ptr == NULL ) {
		ptr = my_alloc ( index * 2 + 2, level - 1, mem );
		if ( ptr == NULL ) {
			return NULL;
		}
	}
	aux->tree_bitmap[index] = 1;
	return ptr;
}

static void my_free_idx ( int index, int * flag, int n, memory_manager_adt mem )
{
	memory_manager_cdt * aux = ( memory_manager_cdt * ) mem;
	if( aux == NULL ){
		return;
	}
	if ( index == 0 ) {
		aux->tree_bitmap[index] = 0;
		return;
	}
	if ( aux->tree_bitmap[index] == 1 && ! ( *flag ) ) {
		aux->free_mem += MIN_BLOCK_SIZE * n;
		*flag = 1;
	}
	aux->tree_bitmap[index] = 0;

	if ( aux->tree_bitmap[GET_SIBLING ( index )] ) { // si el "buddy" está ocupado
		return;
	}
	my_free_idx ( GET_PARENT ( index ), flag, n * 2, mem );
}


static int next_power_of_2 ( int n )
{
	int count = 0;
	if ( n && ! ( n & ( n - 1 ) ) )
		return n;
	while ( n != 0 ) {
		n >>= 1;
		count += 1;
	}
	return 1 << count;
}

static int get_index_level ( int index )
{
	int level = 0;
	for ( int acum = 0; index > acum; level++ ) {
		acum += 2 << level;
	}
	return level;
}

static int get_size_level ( int size )
{
	int level = 0;
	for ( ; 2 * size - 1 < MAX_MEM_SIZE; level++ ) {
		size *= 2;
	}
	return level;
}

static int get_block_from_index ( int index )
{
	int level = get_index_level ( index );
	return ( index + 1 - ( 1 << level ) ) * ( ( MAX_MEM_SIZE / MIN_BLOCK_SIZE ) / ( 1 << level ) );
}

#endif