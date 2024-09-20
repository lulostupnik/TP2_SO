#include <stdlib.h>
#include "syscall.h"
#include "test_util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>

#define BLOCK_SIZE 64
#define BLOCK_COUNT 1024
#define TREE_NODE_COUNT (2 * BLOCK_COUNT - 1)

buddy_node *buddy_tree;
int tree_reserved_bytes;
int size;
int real_size; // Biggest power of 2 smaller than size

typedef struct buddy_node
{
    size_t size;  // Tamaño del bloque
    bool is_free; // Si el bloque está libre
    bool is_split;
    void *memory_addr; // Puntero a la dirección de memoria gestionada
} buddy_node;

int next_2_power(int n);

int prev_2_power(int n);

void *my_malloc(size_t size)
{
}

// Obtener el índice del hijo izquierdo
int left_child(int index)
{
    return 2 * index + 1;
}

// Obtener el índice del hijo derecho
int right_child(int index)
{
    return 2 * index + 2;
}

// Función recursiva para asignar memoria
void *allocate_block(int index, size_t size)
{
    // Si el nodo no tiene tamaño suficiente o no está libre, no es apto
    if (buddy_tree[index].size < size || !buddy_tree[index].is_free)
    {
        return NULL;
    }

    // Si el nodo es del tamaño exacto y está libre, lo asignamos
    if (buddy_tree[index].size < size * 2)
    {
        buddy_tree[index].is_free = false;
        return buddy_tree[index].address;
    }

    // Si no se puede asignar
    return NULL;
}

// Función para asignar memoria
void *my_alloc(size_t size)
{
    // size_t actual_size = next_2_power(size);
    if (size > buddy_tree[0].size)
    {
        return NULL;
    }
    return allocate_block(0, size);
}

void my_free(void *p)
{
    // free_ptrs[--current] = p;
}

void my_mm_init(void *p, int s)
{
    // int real_size = prev_2_power(s);
    buddy_tree = (buddy_node *)p;

    my_malloc(TREE_NODE_COUNT * sizeof(buddy_node));

    // Assign a block for my tree and initialize it
}

int main()
{
    int size = 1000;
    printf("Previous power of 2 for %d is %d\n", size, prev_2_power(size));
}

int next_2_power(int n)
{
    int count = 0;
    while (n != 0)
    {
        n = n >> 1;
        count++;
    }
    return 1 << count;
}

int prev_2_power(int n)
{
    return next_2_power(n) >> 1;
}

// guardar arreglo de punteros a bloques de ciertos tamaños --> no deberían haber varios bloques libres de un mismo tamaño

/*
    // Si el nodo es más grande, lo dividimos y buscamos en los hijos
    if (buddy_tree[index].size > size)
    {
        split_node(index);
        void *left_alloc = allocate_block(left_child(index), size);
        if (left_alloc != NULL)
        {
            return left_alloc;
        }
        return allocate_block(right_child(index), size);
    }
*/