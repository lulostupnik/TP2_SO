// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <process.h>



PCB pcb_array[PCB_AMOUNT] = {0};
uint64_t amount_of_processes = 0;

static int64_t find_free_pcb();
static int64_t set_free_pcb(pid_t pid);



int8_t get_status(pid_t pid){
	PCB * process = get_pcb(pid);
	if (process == NULL) {
		return -1;
	}
	return process->status;
}


//Returns PID if it was succesfull and -1 if it wasnt. 
pid_t wait(pid_t pid, int64_t * ret){

	PCB * pcb_to_wait = get_pcb(pid);
	if((pcb_to_wait == NULL) || (pcb_to_wait->status == FREE) || (pcb_to_wait->waiting_me != NULL) || (pid == get_pid()) /*|| (pcb_to_wait == get_idle_pcb())*/){
		return -1;
	}
	if(!(pcb_to_wait->status == ZOMBIE)){
		PCB * running = get_running();
		pcb_to_wait->waiting_me = running;
		running->waiting_for = pcb_to_wait;
		block_current();
	}
	if(!((pcb_to_wait->status == ZOMBIE))){ // Esto podria pasar si lo mataron
		return -1;
	}
	
	if(ret != NULL){
		*ret = pcb_to_wait->ret;
	}

	if(set_free_pcb(pid) != -1){
		amount_of_processes--;
	}
	return pid;

}



int64_t new_process(main_function rip, priority_t priority, uint8_t killable, char ** argv, uint64_t argc, fd_t fds[])
{

	if (((priority != LOW) && (priority != MEDIUM) && (priority != HIGH))) {
		return -1;
	}

	pid_t pid = find_free_pcb();
	if (pid == -1) {
		return -1;
	}

	uint64_t rsp_malloc = (uint64_t) my_malloc(STACK_SIZE) ;
	uint64_t rsp = rsp_malloc + STACK_SIZE;

	if ((void *)rsp_malloc == NULL) {
		return -1;
	}

	char ** args_cpy = copy_argv(pid, argv, argc);
	if (argc > 0 && args_cpy == NULL) {
		my_free((void *)rsp_malloc);
		pcb_array[pid].status = FREE;
		return -1;
	}


	rsp = load_stack((uint64_t )rip, rsp, args_cpy, argc, pid);

	pcb_array[pid].pid = pid;
	pcb_array[pid].rsp = rsp;
	pcb_array[pid].status = READY;
	pcb_array[pid].argv = args_cpy;
	pcb_array[pid].argc = argc;
	pcb_array[pid].priority = priority;
	pcb_array[pid].killable = killable;
	pcb_array[pid].waiting_me = NULL;
	pcb_array[pid].is_background = 0;
	pcb_array[pid].lowest_stack_address = rsp_malloc;
	pcb_array[pid].blocked_by_sem = -1;

	for(int i = 0; i < 3; i++){
			pcb_array[pid].fds[i] = fds ? fds[i]:-1;
	}

	ready(&pcb_array[pid]);
	amount_of_processes++;
	return pid;
}

static int64_t find_free_pcb()
{
	int64_t to_return = 0;

	while (to_return < PCB_AMOUNT && pcb_array[to_return].status != FREE ) {
		to_return++;
	}

	if (to_return == PCB_AMOUNT) {
		return -1;
	}

	return to_return;
}

PCB * get_pcb(pid_t pid)
{
	if (pid >= PCB_AMOUNT || pid < 0) {
		return NULL;
	}
	return &pcb_array[pid];
}

static int64_t set_free_pcb(pid_t pid)
{
	PCB * process = get_pcb(pid);
	if (process == NULL || process->status == FREE) {
		return -1;
	}
	my_free((void *)process->lowest_stack_address);
	if (process->argv == NULL) {
		process->status = FREE;
		return 0;
	}
	for (uint64_t i = 0; i < process->argc ; i++) {
		my_free((void *)process->argv[i]);
	}
	my_free((void * )process->argv);
	process->status = FREE;
	return 0;
}



int64_t kill_process(pid_t pid)
{	
	PCB * pcb = get_pcb(pid);
	if ( (pcb == NULL) || (pcb->status == FREE) || (!pcb->killable) ) {
		return -1;
	}

	unschedule(pcb);
	unblock_waiting_pid(pid);
	if(pcb->waiting_for && pcb->waiting_for->waiting_me){
		pcb->waiting_for->waiting_me = NULL;
	}
	delete_from_blocked_queue(pcb);
	if(set_free_pcb(pid) != -1){
		amount_of_processes--;
	}

	return 0;
}


void get_process_info(PCB * pcb, process_info * process)
{
	process->name = new_str_copy(pcb->argv != NULL ? pcb->argv[0] : NULL); // Si falla el malloc lo imprimimos como "No name" pero dejamos el resto del estado. 
	process->pid = pcb->pid;
	process->priority = pcb->priority;
	process->stack_pointer = pcb->rsp;
	process->lowest_stack_address = pcb->lowest_stack_address;
	process->status = pcb->status;
	PCB * pcb2;
	process->is_background = !(pcb->fds[STDIN] == STDIN || (pcb->fds[STDIN] > 2 && (pcb2 = (get_pcb(pipe_get_pid(pcb2->fds[STDIN], WRITER)) != NULL)) && pcb2->fds[STDIN] == STDIN) );
	for(int i = 0; i < 3; i++){
		process->fds[i] = pcb->fds ? pcb->fds[i] : -1;
	}
}


process_info_list * ps()
{
	process_info_list * process_list = my_malloc(sizeof(process_info_list));
	if (process_list == NULL) {
		return NULL;
	}
	process_list->amount_of_processes = amount_of_processes;
	process_info * processes = my_malloc(amount_of_processes * sizeof(process_info));
	if (processes == NULL) {
		my_free((void *) process_list);
		return NULL;
	}

	for (int i = 0, found = 0; i < PCB_AMOUNT && found < amount_of_processes; i++) {
		if (pcb_array[i].status != FREE) {
			get_process_info(&pcb_array[i], &processes[found]);
			found++;
		}
	}

	process_list->processes = processes;
	return process_list;
}

void free_ps(process_info_list * ps){
	if(ps == NULL || ps->processes == NULL){
		return;
	}
	for(int i=0; i < ps->amount_of_processes ;i++){
		if(ps->processes[i].name){
			my_free(ps->processes[i].name);
		}
	}
	my_free(ps->processes);
	my_free(ps);
}