include <listADT.h>
uint_8 initialized = 0;

static listADT ready_list;
static listCDT blocked_list;
int compare_elements(elemTypePtr e1, elemTypePtr e2) {
    return e1->pid - e2->pid; 
}

void scheduler(){
    if(!initialized){
        tCompare cmp =compare_elements;
        ready_list=newList(cmp);
        blocked_list=newList(cmp);
        initialized=1;
    }
    if(ready_list->c)

}