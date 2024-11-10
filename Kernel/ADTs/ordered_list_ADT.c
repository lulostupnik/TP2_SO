// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <ordered_list_ADT.h>

typedef struct node{
    elem_type head;
    struct node * tail;
}t_node;

typedef t_node * t_ordered_list;

struct list_cdt {
    t_ordered_list first;   
    size_t size;            
    t_compare cmp;
    t_ordered_list next;    
    t_ordered_list current; 
    t_ordered_list previous;
};


ordered_list_adt new_ordered_list(t_compare cmp){
    ordered_list_adt aux = my_malloc(sizeof(*aux));
    if(aux == NULL){
        return NULL;
    }
    aux->size = 0;
    aux->first = NULL;
    aux->cmp = cmp;
    return aux;
}

static void free_ordered_list_rec(t_ordered_list list){
    if(list == NULL){
        return;
    }
    free_ordered_list_rec(list->tail);
    my_free(list);
}

void free_ordered_list(ordered_list_adt list){
    free_ordered_list_rec(list->first);
    my_free(list);
}

int is_empty_ordered_list(const ordered_list_adt list){
    return list->first == NULL;
}

size_t size_ordered_list(const ordered_list_adt list){
    return list->size;
}




static t_ordered_list add_ordered_list_rec(t_ordered_list list, elem_type elem, t_compare cmp, int * flag){
    int c;
    if(list == NULL || (c = cmp(elem, list->head)) < 0){
        t_ordered_list aux = my_malloc(sizeof(*aux));
        if(aux == NULL){
            *flag = 0;
            return list;
        }
        aux->head = elem;
        aux->tail = list;
        *flag = 1;
        return aux;
    }
    if(c > 0){
        list->tail = add_ordered_list_rec(list->tail, elem, cmp, flag);
    }
    return list;
}

int add_ordered_list(ordered_list_adt list, elem_type elem){
    if(list == NULL || elem == NULL){
        return -1;
    }
    int flag = 0;
    list->first = add_ordered_list_rec(list->first, elem, list->cmp, &flag);
    list->size += flag;
    if(flag == 0){
        return -1;
    }
    return 0;
}

static elem_type get_rec(t_ordered_list l, size_t idx){
    if(idx == 0){
        return l->head;
    }
    return get_rec(l->tail, idx-1);
}

elem_type get(const ordered_list_adt list, size_t idx){
    if( list == NULL || idx >= list->size){
        return NULL;
    }
    return get_rec(list->first, idx);
}


static t_ordered_list delete_rec(t_ordered_list list, elem_type elem, t_compare cmp, int * flag){
    int c;
    if(list==NULL || (c = cmp(list->head, elem)) > 0 ){ 
        *flag = 0;
        return NULL;
    }
    if( c < 0){
        list->tail = delete_rec(list->tail, elem, cmp, flag);
        return list;
    }
    *flag = 1;
    t_ordered_list aux = list->tail;
    my_free(list);
    return aux;

}

int delete_ordered_list(ordered_list_adt list, elem_type elem){
    if(list == NULL || list->first == NULL || elem == NULL){
        return -1;
    }
    int aux;
    list->first = delete_rec(list->first, elem, list->cmp, &aux);
    list->size -= aux;
    return !aux;
}


void ordered_list_to_begin(ordered_list_adt list) {
    list->next = list->first;
    list->current = NULL;
    list->previous = NULL;
}


int ordered_list_has_next(const ordered_list_adt list){
    return list->next != NULL;
}


elem_type ordered_list_next(ordered_list_adt list) {
    if (!ordered_list_has_next(list)) {
        return NULL;
    }
    list->previous = list->current;
    list->current = list->next;
    list->next = list->next->tail;
    return list->current->head;
}

int ordered_list_delete_current(ordered_list_adt list) {
    if (list->current == NULL) {
        return -1;
    }

    if (list->previous == NULL) {
        list->first = list->next; 
    } else {
        list->previous->tail = list->next;  
    }

    my_free(list->current);
    list->size--;

    list->current = NULL;

    return 0;
}

