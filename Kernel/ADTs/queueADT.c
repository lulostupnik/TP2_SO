// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <queueADT.h>

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
    queue_adt to_return = my_malloc(sizeof(*to_return));
    if(to_return == NULL){
        return NULL;
    }
    to_return->size = 0;
    to_return->first = NULL;
    to_return->last = NULL;
    return to_return;
}

uint64_t queue_is_empty(queue_adt queue){
    if(queue == NULL){
        // @todo -> manejar error
        return 0;
    }
    return (queue->size == 0);
}

elem_type_ptr dequeue(queue_adt queue){
    if(queue == NULL || queue_is_empty(queue)){
        return NULL;
    }
    queue->size--;
    elem_type_ptr to_return = queue->first->head;
    t_queue aux = queue->first->tail;
    my_free(queue->first);
    queue->first = aux;
    if(aux == NULL){
        queue->last = NULL;
    }
    return to_return;
}

void enqueue(queue_adt queue, elem_type_ptr head){
    if(queue == NULL){
        // @todo -> ¿manejar error?
        return;
    }
    t_queue new_node = my_malloc(sizeof(*new_node));
    if(new_node == NULL){
        // @todo -> ¿manejar error?
        return;
    }
    new_node->head = head;
    new_node->tail = NULL;
    if(queue_is_empty(queue)){
        queue->first = new_node;
        queue->last = new_node;
        queue->size++;
        return;
    }
    queue->size++;
    queue->last->tail = new_node;
    queue->last = new_node;
}

void free_queue(queue_adt queue){ //@todo check mem leak
    if(queue == NULL){
        return;
    }
    while(!queue_is_empty(queue)){
        dequeue(queue);
    }
    my_free(queue);
}


