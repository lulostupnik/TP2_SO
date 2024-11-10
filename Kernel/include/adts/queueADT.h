#ifndef _QUEUEADT_H_
#define _QUEUEADT_H_

#include <stddef.h>
#include <stdint.h>
#include <PCB.h>
#include <memory_management.h>

typedef struct queue_cdt * queue_adt;
typedef PCB * elem_type_ptr;

typedef int (*t_compare)(elem_type_ptr e1, elem_type_ptr e2);
queue_adt new_queue();
uint64_t queue_is_empty(queue_adt queue);
elem_type_ptr dequeue(queue_adt queue);
void enqueue(queue_adt queue, elem_type_ptr value);
void free_queue(queue_adt queue);
int64_t delete_from_queue(queue_adt queue, elem_type_ptr head);

#endif