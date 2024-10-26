#include <ordered_list_ADT.h>

typedef struct node{
    elem_type head;
    struct node * tail;
}t_node;

typedef t_node * t_ordered_list;

struct list_cdt {
    t_ordered_list first;   // puntero al primer nodo
    size_t size;            // cant de elementos en la lista
    t_compare cmp;
    t_ordered_list next;
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


int belongs_ordered_list(const ordered_list_adt list, elem_type elem){
    if((list == NULL) || (list->first == NULL)){
        return -1;
    }
    t_ordered_list aux = list->first;
    int comparacion; 
    while((aux != NULL) && ((comparacion = list->cmp(elem, aux->head)) <= 0)){
        //cmp retorna 0 si son iguales
        if(!comparacion){
            return 0;
        }
        aux = aux->tail;
    }
    return -1;
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
    if(list == NULL){
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
    if( idx >= list->size){
        return NULL;
    }
    return get_rec(list->first, idx);
}


static t_ordered_list delete_rec(t_ordered_list list, elem_type elem, t_compare cmp, int * flag){
    int c;
    if(list==NULL || (c = cmp(list->head, elem)) > 0 ){ // cmp(e1,e2) > 0 si e1 esta despues que e2
        *flag = 0;
        return NULL;
    }
    if( c < 0){
        list->tail = delete_rec(list->tail, elem, cmp, flag);
        return list;
    }
    //son iguales
    *flag = 1;
    t_ordered_list aux = list->tail;
    my_free(list);
    return aux;

}

int delete_ordered_list(ordered_list_adt list, elem_type elem){
    int aux;
    list->first = delete_rec(list->first, elem, list->cmp, &aux);
    list->size -= aux;
    return aux;
}


void ordered_list_to_begin(ordered_list_adt list){
    list->next = list->first;
}

int ordered_list_has_next(const ordered_list_adt list){
    return list->next != NULL;
}

elem_type ordered_list_next(ordered_list_adt list){
    if(!ordered_list_has_next(list)){
        return NULL;
    }
    elem_type aux = list->next->head;
    list->next = list->next->tail;
    return aux;
}
