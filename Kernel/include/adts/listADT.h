
//listas ordenadas sin repetidos de cualquier tipo

#ifndef _LISTADT_H
#define LIST_ADT_H

#include <stdio.h>
#include <stddef.h>
#include <assert.h>
#include <memory_management.h>
#include <process.h>
//ElemType must be a pointer

//@TODO podria ser un elemType sin puntero y el get y esos devuelven el puntero

typedef struct listCDT * listADT;
typedef PCB * elemTypePtr;

/*
*  < 0 si e1 debe estar antes que e2 (aka e1< e2, en caso ascendente)
*  > 0 si e1 debe estar despues que e2 (aka e1 > e2)
*  0 si son "iguales" 
*/

typedef int (*tCompare)(elemTypePtr e1, elemTypePtr e2);

listADT newList(tCompare cmp);

void freeList(listADT list);

int addList(listADT list, elemTypePtr elem); // retorna 1 si lo inserto, 0 si no.

int deleteList(listADT list, elemTypePtr elem);

int sizeList(const listADT list);


int isEmptyList(const listADT list);

int belongsList(const listADT list, elemTypePtr elem);

elemTypePtr get(const listADT list, size_t idx);

//void printList(listADT list);

//Le avisamos que vamos a recorrer la lista desde el primero
int toBegin(listADT list); 

//Retorna true si hay pendientes que recorrer
int hasNext(const listADT list);

//Me devuelve el siguiente. Precondicion, hasNext debe devolver true
elemTypePtr next(listADT list);


#endif