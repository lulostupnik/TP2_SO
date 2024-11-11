// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <queue_adt.h>

typedef struct node {
	elem_type_ptr head;
	struct node * tail;
} t_node;

typedef t_node * t_queue;

struct queue_cdt {
	t_queue first;
	t_queue last;
	uint64_t size;
};

queue_adt new_queue()
{
	queue_adt to_return = my_malloc ( sizeof ( *to_return ), get_kernel_mem() );
	if ( to_return == NULL ) {
		return NULL;
	}
	to_return->size = 0;
	to_return->first = NULL;
	to_return->last = NULL;
	return to_return;
}

uint64_t queue_is_empty ( queue_adt queue )
{
	if ( queue == NULL ) {
		return 1;
	}
	return ( queue->size == 0 );
}

elem_type_ptr dequeue ( queue_adt queue )
{
	if ( queue_is_empty ( queue ) ) {
		return NULL;
	}
	queue->size--;
	elem_type_ptr to_return = queue->first->head;
	t_queue aux = queue->first->tail;
	my_free ( queue->first, get_kernel_mem() );
	queue->first = aux;
	if ( aux == NULL ) {
		queue->last = NULL;
	}
	return to_return;
}

int64_t delete_from_queue ( queue_adt queue, elem_type_ptr head )
{
	if ( queue_is_empty ( queue ) ) {
		return -1;
	}

	t_queue current = queue->first;
	t_queue previous = NULL;

	while ( current != NULL && current->head != head ) {
		previous = current;
		current = current->tail;
	}

	if ( current == NULL ) {
		return -1;
	}

	if ( previous == NULL ) {
		queue->first = current->tail;
	} else {
		previous->tail = current->tail;
	}

	if ( current->tail == NULL ) {
		queue->last = previous;
	}

	my_free ( current, get_kernel_mem() );
	queue->size--;

	return 0;
}




int64_t enqueue ( queue_adt queue, elem_type_ptr head )
{
	if ( queue == NULL ) {
		return -1;
	}
	t_queue new_node = my_malloc ( sizeof ( *new_node ),  get_kernel_mem() );
	if ( new_node == NULL ) {
		return -1;
	}
	new_node->head = head;
	new_node->tail = NULL;
	if ( queue_is_empty ( queue ) ) {
		queue->first = new_node;
		queue->last = new_node;
		queue->size++;
		return 0;
	}
	queue->size++;
	queue->last->tail = new_node;
	queue->last = new_node;
	return 0;
}

void free_queue ( queue_adt queue )
{
	if ( queue == NULL ) {
		return;
	}
	while ( !queue_is_empty ( queue ) ) {
		dequeue ( queue );
	}
	my_free ( queue, get_kernel_mem() );
}


