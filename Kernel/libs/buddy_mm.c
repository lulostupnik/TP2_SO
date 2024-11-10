#ifdef BUDDY
#include <memory_management.h>

#define GET_SIBLING(i) (i % 2 ? i + 1 : i - 1)
#define GET_PARENT(i) ((i - 1) / 2) // todo check

#define MAX_MEM_SIZE HEAP_SIZE 
#define MIN_BLOCK_SIZE BLOCK_SIZE 

#define TREE_BIT_MAP_SIZE (((MAX_MEM_SIZE / MIN_BLOCK_SIZE) / 8) * 2)

#define TREE_BIT_MAP_SIZE (((MAX_MEM_SIZE / MIN_BLOCK_SIZE)) * 2)

uint64_t free_mem = MAX_MEM_SIZE;

int next_power_of_2(int n);
int get_index_level(int index);
int get_size_level(int size);
int get_block_from_index(int index);

char tree_bitmap[TREE_BIT_MAP_SIZE]; 

void *start;

int64_t my_mem_info(memory_info *info)
{
    if(info == NULL){
        return -1;
    }
    info->total_size = MAX_MEM_SIZE;
    info->free = free_mem;
    return 0;
}

void my_mm_init ( void * p )
{
    start = p;
    
	for(int i = 0; i < TREE_BIT_MAP_SIZE; i++){
		tree_bitmap[i] = 0;
	}
}

void *my_alloc(int index, int level)
{
    if (level == 0)
    {
        if (tree_bitmap[index])
        {
            return NULL;
        }
        tree_bitmap[index] = 1;
        return start + get_block_from_index(index) * MIN_BLOCK_SIZE; // todo -> MAL -> debería ser start + get_block_from_index(index) * MIN_BLOCK_SIZE
    }
    if (tree_bitmap[index] && !tree_bitmap[2 * index + 1] && !tree_bitmap[index * 2 + 2])
    {
        return NULL;
    }
    // esto puesto que si el bloque está ocupado, pero sus hijos están libres, significa que el bloque no está dividido, sino que está ocupado

    void *ptr = my_alloc(index * 2 + 1, level - 1);
    if (ptr == NULL)
    {
        ptr = my_alloc(index * 2 + 2, level - 1);
        if (ptr == NULL)
        {
            return NULL;
        }
    }
    tree_bitmap[index] = 1;
    return ptr;
}

void * my_malloc ( uint64_t size )
{
    int npo2 = next_power_of_2(size);
    int real_size = npo2 >= MIN_BLOCK_SIZE ? npo2 : MIN_BLOCK_SIZE;
    int level = get_size_level(real_size);

    void * ptr = my_alloc(0, level);
    if(ptr != NULL){
        free_mem -= real_size;
    }

    return ptr;
}

void my_free_idx(int index, int * flag, int n)
{
    if (index == 0)
    {
        tree_bitmap[index] = 0;
        return;
    }
    if(tree_bitmap[index] == 1 && !(*flag)){
        free_mem += MIN_BLOCK_SIZE * n;
        *flag = 1;
    }
    tree_bitmap[index] = 0;

    if (tree_bitmap[GET_SIBLING(index)]) // si el "buddy" está ocupado
    {
        return;
    }
    my_free_idx(GET_PARENT(index), flag, n * 2 );
}

void my_free ( void * p )
{
    if ((p - start) % MIN_BLOCK_SIZE != 0)
    {
        return;
    }

    int index = ((p - start) / MIN_BLOCK_SIZE) + MAX_MEM_SIZE / MIN_BLOCK_SIZE - 1; // arrancamos por el bloque de granularidad maxima
    int flag = 0;
    int n = 1;
    my_free_idx(index, &flag, n);
}

int next_power_of_2(int n)
{
    int count = 0;
    if (n && !(n & (n - 1)))
        return n;
    while (n != 0)
    {
        n >>= 1;
        count += 1;
    }
    return 1 << count;
}

int get_index_level(int index)
{
    int level = 0;
    for (int acum = 0; index > acum; level++)
    {
        acum += 2 << level;
    }
    return level;
}

int get_size_level(int size)
{
    int level = 0;
    for (; 2 * size - 1 < MAX_MEM_SIZE; level++)
    {
        size *= 2;
    }
    return level;
}

int get_block_from_index(int index)
{
    int level = get_index_level(index);
    return (index + 1 - (1 << level)) * ((MAX_MEM_SIZE / MIN_BLOCK_SIZE) / (1 << level));
}

#endif