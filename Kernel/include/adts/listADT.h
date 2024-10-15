#ifndef _LISTADT_H_
#define _LISTADT_H_


#include <stddef.h>
#include <assert.h>
#include <memory_management.h>
#include <process.h>
#include <PCB.h>

typedef struct list_cdt * list_adt;
typedef PCB * elem_type_ptr;

/*
*  < 0 si e1 debe estar antes que e2 (aka e1< e2, en caso ascendente)
*  > 0 si e1 debe estar despues que e2 (aka e1 > e2)
*  0 si son "iguales"
*/


typedef int (*t_compare)(elem_type_ptr e1, elem_type_ptr e2);

list_adt new_list(t_compare cmp);

void free_list(list_adt list);

int add_list(list_adt list, elem_type_ptr elem);

int delete_list(list_adt list, elem_type_ptr elem);

int size_list(const list_adt list);

int is_empty_list(const list_adt list);

elem_type_ptr next(list_adt list);



#endif