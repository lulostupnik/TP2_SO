// this is a personal academic project. dear pvs-studio, please check it.
// pvs-studio static code analyzer for c, c++ and c#: http://www.viva64.com

#include <time.h>
extern void _hlt();
static unsigned long ticks = 0;

static ordered_list_adt sleeping_list;

int compare(PCB * pcb1, PCB * pcb2){
	return ((pcb1->start + pcb1->time) < (pcb2->start + pcb2->time)) ? -1:1;
}

void init_timer_handler(){
	sleeping_list = new_ordered_list(compare);
}


void timer_handler()
{
	ticks++;
	if(sleeping_list == NULL){
		return;
	}
	ordered_list_to_begin(sleeping_list);
	while(ordered_list_has_next(sleeping_list)){
		PCB * current_pcb = ordered_list_next(sleeping_list);
		if(current_pcb == NULL || ticks - current_pcb->start < current_pcb->time){
			return;
		}
		ready(current_pcb);
		ordered_list_delete_current(sleeping_list);
	}
}

int ticks_elapsed()
{
	return ticks;
}

int seconds_elapsed()
{
	return ticks / 18;
}

void nano_sleep ( int time )
{	
	PCB * pcb  = get_running();
	pcb->start = ticks;
	pcb->time = time;
	add_ordered_list(sleeping_list, pcb); //@todo handle error
	block_current();
	pcb->start = 0;
	pcb->time = 0;
}