#include <listADT.h>



typedef struct node {
    elemTypePtr head;
    struct node *tail;
} tNode;

typedef tNode *tList;

struct listCDT {
    tList first;  // Pointer to the first node
    tList last;
    size_t size;  // Number of elements in the list
    tCompare cmp; // Comparison function for elements
    tList next;   // Pointer for iteration
};

// Create a new list
listADT newList(tCompare cmp) {
    listADT aux = my_malloc(sizeof(*aux));
    if(aux == NULL){
        return NULL;
    } // Ensure memory allocation is successful
    aux->size = 0;
    aux->first = NULL;
    aux->last = NULL;
    aux->cmp = cmp;
    return aux;
}

// Free the list recursively
static void freeListRec(tList list) {
    if (list == NULL) {
        return;
    }
    freeListRec(list->tail);
    my_free(list);
}

void freeList(listADT list) {
    if(list == NULL){
        return;
    }
    freeListRec(list->first);
    my_free(list);
}

int isEmptyList(const listADT list) {   
    return list == NULL || list->first == NULL;
}

int sizeList(const listADT list) {
    if(list == NULL){
        return -1;
    }
    return list->size;
}

int addList(listADT list, elemTypePtr elem) {
    if (list == NULL) {
        return -1
        
        ;
    }

    tList newNode = my_malloc(sizeof(*newNode));
    if (newNode == NULL) {
        return -1; 
    }

    newNode->head = elem;
    newNode->tail = NULL;

    if (list->first == NULL) {
        list->first = newNode;
        list->last = newNode;
    } else {
        list->last->tail = newNode;
        list->last = newNode;
    }

    list->size++;

    return 0; // Successfully added
}


// Check if an element belongs to the list
static int belongsRec(tList l, elemTypePtr elem, tCompare cmp) {
    if (l == NULL) {
        return 0;
    }
    if (cmp(elem, l->head) == 0) {
        return 1; // Element found
    }
    return belongsRec(l->tail, elem, cmp);
}

int belongsList(const listADT list, elemTypePtr elem) {
    if(list == NULL){
        return -1;
    }
    return belongsRec(list->first, elem, list->cmp);
}

static elemTypePtr getRec(tList l, size_t idx) {
    if (idx == 0) {
        return l->head;
    }
    return getRec(l->tail, idx - 1);
}

elemTypePtr get(const listADT list, size_t idx) {
    if(list == NULL || idx >= list->size){
        return NULL;
    }
    return getRec(list->first, idx);
}
//1744
// Set the list iterator to the beginning
int toBegin(listADT list) {
    if(list == NULL){
        return -1;
    }
    list->next = list->first;
    return 0;
}

// Check if there is a next element
int hasNext(const listADT list) {
    return list != NULL && list->next != NULL;
}

// Get the next element in the list
elemTypePtr next(listADT list) {
    if (!hasNext(list)) {
        return NULL;
    }
    elemTypePtr aux = list->next->head;
    list->next = list->next->tail;
    return aux;
}

// Delete an element from the list (delete only one occurrence)
static tList deleteRec(tList list, elemTypePtr elem, tCompare cmp, int *flag) {
    if (list == NULL) {
        *flag = 0;
        return NULL;
    }
    if (cmp(elem, list->head) == 0) {
        *flag = 1;
        tList aux = list->tail;
        my_free(list);
        return aux;
    }
    list->tail = deleteRec(list->tail, elem, cmp, flag);
    return list;
}

int deleteList(listADT list, elemTypePtr elem) {
    if(list == NULL){
        return -1;
    }
    int flag;
    list->first = deleteRec(list->first, elem, list->cmp, &flag);
    list->size -= flag;
    return flag;
}



// Print the elements of the list
/*void printList(listADT list) {
    assert(list != NULL); // Ensure the list is valid
    putchar('{');
    tList current = list->first;
    while (current != NULL) {
        printf("%d", current->head);
        if (current->tail != NULL) {
            printf(", ");
        }
        current = current->tail;
    }
    putchar('}');
    putchar('\n');
}*/
