#include <listADT.h>





typedef struct node {
    elem_type_ptr head;
    // int has_runned_times; //no admite repetidos
    struct node *tail;
} t_node;

typedef t_node *t_list;

struct list_cdt {
    // t_list first;  // pointer to the first node
    t_list pre_next;
    size_t size;  // number of elements in the list
    t_compare cmp; // comparison function for elements
    // t_list next;   // pointer for iteration
};

// create a new list
list_adt new_list(t_compare cmp) {
    list_adt aux = my_malloc(sizeof(*aux));
    if(aux == NULL){
        return NULL;
    } // ensure memory allocation is successful
    aux->size = 0;
    aux->pre_next = NULL;
    aux->cmp = cmp;
    return aux;
}


void free_list(list_adt list) {
    if(list == NULL){
        return;
    }
    for(int i = 0; i < list->size; i++){
        t_list aux = list->pre_next;
        list->pre_next = list->pre_next->tail;
        my_free(aux);
    }
    my_free(list);
}

int is_empty_list(const list_adt list) {   
    return list == NULL || list->pre_next == NULL;
}

int size_list(const list_adt list) {
    if(list == NULL){
        return -1;
    }
    return list->size;
}

int add_list(list_adt list, elem_type_ptr elem) {
    if (list == NULL) {
        return -1;
    }

    t_list new_node = my_malloc(sizeof(*new_node));
    if (new_node == NULL) {
        return -1; 
    }

    new_node->head = elem;
   
    if (list->pre_next == NULL) {
       list->pre_next = new_node;
       list->pre_next->tail = new_node; 
    } else {
       new_node->tail = list->pre_next->tail;
       list->pre_next->tail = new_node;
       list->pre_next = new_node;
    } 

    list->size++;
    return 0; // successfully added
}

// get the next element in the list
elem_type_ptr next(list_adt list) {
    if (is_empty_list(list)) {
        return NULL;
    }

    elem_type_ptr aux = list->pre_next->tail->head;
    list->pre_next = list->pre_next->tail;
    return aux;
}


int delete_list(list_adt list, elem_type_ptr elem)
{
    if (is_empty_list(list))
    {
        return -1;
    }
    if (list->size == 1)
    {
        if (list->cmp(elem, list->pre_next->head) != 0)
        {
            return -1;
        }
        my_free(list->pre_next);
        list->pre_next = NULL;
        list->size = 0;
        return 0;
    }

    t_list ant = list->pre_next;
    t_list current = list->pre_next->tail;
    for(int i=0; i<list->size ; i++){
        if(list->cmp(elem, current->head) == 0){
            ant->tail = current->tail;
            if(list->pre_next == current){
                list->pre_next = ant;
            }
            my_free(current);
            list->size -- ;
            return 0;
        }
        ant = ant->tail;
        current = current->tail;
    }
    return -1;
}
