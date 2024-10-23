// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <scheduler.h>

static list_adt ready_list;
static list_adt blocked_list;

static PCB * running = NULL;
static uint64_t times_ran = 0;

static PCB * idle_pcb;
static int initialized = 0;

int compare_elements(elem_type_ptr e1, elem_type_ptr e2)
{
	return e1 - e2;
}

void initialize_scheduler(int64_t idle_process_pid)
{
	t_compare cmp = compare_elements;
	ready_list = new_list(cmp);
	blocked_list = new_list(cmp);
	idle_pcb = get_pcb(idle_process_pid);
	initialized = 1;
	return;
}

void ready(PCB * process)
{
	process->status = READY;
	add_list(ready_list, process);
	delete_list(blocked_list, process);

}

void block(PCB * process)
{
	process->status = BLOCKED;
	delete_list(ready_list, process);
	add_list(blocked_list, process);
	if(process == running){
		scheduler_yield();
	}
}

void block_current(){
	block(running);
}

void unblock_waiting_me(){
	PCB * pcb = running;
	if(pcb == NULL || pcb->waiting_me == NULL){
		return;
	}
	ready(pcb->waiting_me);
}

void unblock_waiting_pid(pid_t pid){
	PCB * pcb = get_pcb(pid);
	if(pcb == NULL || pcb->waiting_me == NULL){
		return;
	}
	ready(pcb->waiting_me);
}



int64_t make_me_zombie(int64_t retval){
	PCB * pcb = get_running();
	if ( (pcb == NULL) || (pcb->status == FREE) ) {
		return -1;
	}
	pcb->ret = retval;
	unschedule(pcb);
	unblock_waiting_me();
	pcb->status = ZOMBIE;
}


PCB * get_running(){
	return running;
}

void unschedule(PCB * process)
{
	if (process->status == READY) {
		delete_list(ready_list, process);
	} else if (process->status == BLOCKED) {
		delete_list(blocked_list, process);
	}

}

void scheduler_yield()
{
	if (running == NULL || is_empty_list(ready_list)) {
		return;
	}
	times_ran = running->priority;
	timer_tick();
}


int64_t get_pid()
{
	return running->pid;
}

uint64_t block_arbitrary(pid_t pid)
{
	PCB * process = get_pcb(pid);
	if (process == NULL) {
		return -1;
	}
	block(process);
	return 0;
}

uint64_t unblock_arbitrary(pid_t pid)
{
	PCB * process = get_pcb(pid);
	if (process == NULL) {
		return -1;
	}
	ready(process);
	return 0;

}

uint64_t scheduler(uint64_t current_rsp)
{
	if (!initialized) {
		return current_rsp;
	}
	if (running != NULL) {
		running->rsp = current_rsp;
	}
	if (is_empty_list(ready_list)) {
		running = idle_pcb;
		return idle_pcb->rsp;
	}
	if (running == NULL) {
		running = next(ready_list);
		return running->rsp;
	}
	if (times_ran >= running->priority || running->status == ZOMBIE) {
		PCB * next_pcb = next(ready_list);
		times_ran = 0;
		running = next_pcb;
		return next_pcb->rsp;
	}
	times_ran++;
	return current_rsp;
}

int64_t nice ( pid_t pid, uint64_t new_prio )
{
	PCB * process = get_pcb(pid);
	if (process == NULL) {
		return -1;
	}
	process->priority = new_prio;
	return 0;
}
