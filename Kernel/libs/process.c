// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <process.h>



PCB pcb_array[PCB_AMOUNT] = {0};
uint64_t amount_of_processes = 0;

static int64_t find_free_pcb();
static int64_t set_free_pid ( pid_t pid );
static int64_t set_free_pcb ( PCB * process );
static int64_t setup_pipe ( pid_t pid, fd_t fds[] );

int8_t get_status ( pid_t pid )
{
	PCB * process = get_pcb ( pid );
	if ( process == NULL ) {
		return -1;
	}
	return process->status;
}


//Returns PID if it was succesfull and -1 if it wasnt.
pid_t wait ( pid_t pid, int64_t * ret )
{

	PCB * pcb_to_wait = get_pcb ( pid );
	if ( ( pcb_to_wait == NULL ) || ( pcb_to_wait->status == FREE ) || ( pcb_to_wait->waiting_me != NULL ) || ( pid == get_pid() ) ) {
		return -1;
	}
	if ( ! ( pcb_to_wait->status == ZOMBIE ) ) {
		PCB * running = get_running();
		pcb_to_wait->waiting_me = running;
		running->waiting_for = pcb_to_wait;
		block_current();
		running->waiting_for = NULL;
	}
	if ( ! ( ( pcb_to_wait->status == ZOMBIE ) ) ) { // This could happen if it was killed
		return -1;
	}

	if ( ret != NULL ) {
		*ret = pcb_to_wait->ret;
	}

	if ( set_free_pid ( pid ) != -1 ) {
		amount_of_processes--;
	}
	return pid;

}



int64_t new_process ( main_function rip, priority_t priority, uint8_t killable, char ** argv, uint64_t argc, fd_t fds[] )
{

	if ( ( ( priority != LOW ) && ( priority != MEDIUM ) && ( priority != HIGH ) ) ) {
		return -1;
	}

	pid_t pid = find_free_pcb();
	if ( pid == -1 ) {
		return -1;
	}

	uint64_t rsp_malloc = ( uint64_t ) my_malloc ( STACK_SIZE,  get_kernel_mem() ) ;
	uint64_t rsp = rsp_malloc + STACK_SIZE;

	if ( ( void * ) rsp_malloc == NULL ) {
		return -1;
	}

	char ** args_cpy = copy_argv ( pid, argv, argc );
	if ( argc > 0 && args_cpy == NULL ) {
		my_free ( ( void * ) rsp_malloc, get_kernel_mem() );
		pcb_array[pid].status = FREE;
		return -1;
	}


	rsp = load_stack ( ( uint64_t ) rip, rsp, args_cpy, argc, pid );

	pcb_array[pid].pid = pid;
	pcb_array[pid].rsp = rsp;
	pcb_array[pid].status = READY;
	pcb_array[pid].argv = args_cpy;
	pcb_array[pid].argc = argc;
	pcb_array[pid].priority = priority;
	pcb_array[pid].killable = killable;
	pcb_array[pid].waiting_me = NULL;
	pcb_array[pid].lowest_stack_address = rsp_malloc;
	pcb_array[pid].blocked_by_sem = -1;
	for ( int i = 0; i < 3; i++ ) {
		pcb_array[pid].fds[i] = fds ? fds[i] : -1;
	}

	if ( setup_pipe ( pid, fds ) == -1 ) {
		my_free ( ( void * ) rsp_malloc, get_kernel_mem() );
		for ( uint64_t i = 0; i < pcb_array[pid].argc ; i++ ) {
			my_free ( ( void * ) pcb_array[pid].argv[i], get_kernel_mem() );
		}
		my_free ( ( void * ) pcb_array[pid].argv, get_kernel_mem() );
		pcb_array[pid].status = FREE;
		return -1;
	}

	ready ( &pcb_array[pid] );
	amount_of_processes++;
	return pid;
}


static int64_t setup_pipe ( pid_t pid, fd_t fds[] )
{
	if ( !fds ) {
		return 0;
	}
	for ( int i = 0; i < CANT_FDS; i++ ) {
		if ( fds[i] <= MAX_COMMON_FD ) {
			continue;
		}
		pipe_mode_t mode = i == STDIN ? READER : WRITER;
		if ( pipe_open_pid ( fds[i] - 3, mode, pid ) == -1 ) {
			for ( int j = 0; j < i ; j++ ) {
				pipe_close ( fds[i] - 3, pid );
			}
			return -1;
		}
	}
	return 0;
}

static int64_t find_free_pcb()
{
	int64_t to_return = 0;

	while ( to_return < PCB_AMOUNT && pcb_array[to_return].status != FREE ) {
		to_return++;
	}

	if ( to_return == PCB_AMOUNT ) {
		return -1;
	}

	return to_return;
}

PCB * get_pcb ( pid_t pid )
{
	if ( pid >= PCB_AMOUNT || pid < 0 ) {
		return NULL;
	}
	return &pcb_array[pid];
}

static int64_t set_free_pcb ( PCB * process )
{
	if ( process == NULL || process->status == FREE ) {
		return -1;
	}
	close_fds ( process );

	my_free ( ( void * ) process->lowest_stack_address, get_kernel_mem() );
	if ( process->argv == NULL ) {
		process->status = FREE;
		return 0;
	}
	for ( uint64_t i = 0; i < process->argc ; i++ ) {
		my_free ( ( void * ) process->argv[i], get_kernel_mem() );
	}
	my_free ( ( void * ) process->argv, get_kernel_mem() );
	process->status = FREE;

	return 0;
}

static int64_t set_free_pid ( pid_t pid )
{
	PCB * process = get_pcb ( pid );
	return set_free_pcb ( process );
}

int64_t kill_process_pcb ( PCB * pcb )
{
	if ( ( pcb == NULL ) || ( pcb->status == FREE ) || ( !pcb->killable ) ) {
		return -1;
	}

	unschedule ( pcb );
	unblock_waiting_pcb ( pcb );
	if ( pcb->waiting_for && pcb->waiting_for->waiting_me ) {
		pcb->waiting_for->waiting_me = NULL;
	}

	if ( get_keyboard_blocked() == pcb ) {
		set_keyboard_blocked_null();
	}
	if ( pcb->time != 0 || pcb->start != 0 ) {
		unsleep_kill ( pcb );
	}

	sem_delete_from_blocked_queue ( pcb );
	if ( set_free_pcb ( pcb ) != -1 ) {
		amount_of_processes--;
	}

	if ( pcb == get_running() ) {
		set_running_null();
		timer_tick();
	}

	return 0;
}


int64_t kill_process ( pid_t pid )
{
	PCB * pcb = get_pcb ( pid );
	return kill_process_pcb ( pcb );
}


static int64_t is_foreground ( PCB * pcb )
{
	if ( pcb == NULL ) {
		return 0;
	}
	PCB * shell_pcb = get_shell_pcb();
	if ( shell_pcb == NULL ) {
		return 0;
	}
	PCB * fore1 = shell_pcb->waiting_for;
	if ( fore1 == NULL ) {
		if ( pcb == shell_pcb ) {
			return 1;
		}
		return 0;
	}
	if ( fore1 == pcb ) {
		return 1;
	}

	PCB * fore2 = NULL;
	if ( fore1->fds[STDIN] > MAX_COMMON_FD ) {
		fore2 = get_pcb ( pipe_get_pid ( fore1->fds[STDIN] - 3, WRITER ) );
	}
	if ( fore2 != NULL && fore2 == pcb ) {
		return 1;
	}
	return 0;
}

void get_process_info ( PCB * pcb, process_info * process )
{
	process->name = new_str_copy ( pcb->argv != NULL ? pcb->argv[0] : NULL ); // If malloc fails, we print it as "No name" but keep the rest of the state.
	process->pid = pcb->pid;
	process->priority = pcb->priority;
	process->stack_pointer = pcb->rsp;
	process->lowest_stack_address = pcb->lowest_stack_address;
	process->status = pcb->status;
	process->is_background = !is_foreground ( pcb );
	for ( int i = 0; i < 3; i++ ) {
		process->fds[i] = pcb->fds[i] ? pcb->fds[i] : -1;
	}
}


process_info_list * ps()
{
	process_info_list * process_list = my_malloc ( sizeof ( process_info_list ),  get_kernel_mem() );
	if ( process_list == NULL ) {
		return NULL;
	}
	process_list->amount_of_processes = amount_of_processes;
	process_info * processes = my_malloc ( amount_of_processes * sizeof ( process_info ), get_kernel_mem() );
	if ( processes == NULL ) {
		my_free ( ( void * ) process_list, get_kernel_mem() );
		return NULL;
	}

	for ( int i = 0, found = 0; i < PCB_AMOUNT && found < amount_of_processes; i++ ) {
		if ( pcb_array[i].status != FREE ) {
			get_process_info ( &pcb_array[i], &processes[found] );
			found++;
		}
	}

	process_list->processes = processes;
	return process_list;
}

void free_ps ( process_info_list * ps )
{
	if ( ps == NULL || ps->processes == NULL ) {
		return;
	}
	for ( int i = 0; i < ps->amount_of_processes ; i++ ) {
		if ( ps->processes[i].name ) {
			my_free ( ps->processes[i].name,  get_kernel_mem() );
		}
	}
	my_free ( ps->processes, get_kernel_mem() );
	my_free ( ps, get_kernel_mem() );
}

void close_fds ( PCB * pcb )
{
	if ( pcb == NULL ) {
		return;
	}
	for ( int i = 0; i < CANT_FDS ; i++ ) {
		if ( pcb->fds[i] > MAX_COMMON_FD ) {
			pipe_close ( pcb->fds[i] - CANT_FDS, pcb->pid );
		}
	}
}

int64_t make_me_zombie ( int64_t retval )
{
	PCB * pcb = get_running();
	if ( ( pcb == NULL ) || ( pcb->status == FREE ) ) {
		return -1;
	}
	pcb->ret = retval;
	unschedule ( pcb );
	pcb->status = ZOMBIE;
	unblock_waiting_me();
	close_fds ( pcb );
	return 0;
}



void ctrl_c_handler()
{
	PCB * shell_pcb = get_shell_pcb();
	PCB * foreground_process;
	if ( shell_pcb == NULL || ( ( foreground_process = shell_pcb->waiting_for ) == NULL ) ) {
		return;
	}
	// first, we wait on the right side of the pipe.
	PCB * other_process_in_pipe = NULL;
	if ( foreground_process->fds[STDIN] > MAX_COMMON_FD ) {
		other_process_in_pipe = get_pcb ( pipe_get_pid ( foreground_process->fds[STDIN] - 3, WRITER ) );
	}
	kill_process_pcb ( foreground_process );
	kill_process_pcb ( other_process_in_pipe );
	return;
}


