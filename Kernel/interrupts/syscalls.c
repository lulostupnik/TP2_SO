// this is a personal academic project. dear pvs-studio, please check it.
// pvs-studio static code analyzer for c, c++ and c#: http://www.viva64.com

#include <syscallsKernel.h>


static int64_t sys_free_ps_wrapper(process_info_list * ps);
static int64_t sys_free_wrapper ( void * p );

extern uint64_t regs_shot[17];
extern uint64_t regs_shot_available;


int64_t sys_call_handler ( stack_registers * regs )
{
	switch ( regs->rax ) {
		case 0:
			return sys_read ( ( uint16_t * ) regs->rdi,  regs->rsi);
		case 1:
			return sys_write ( regs->rdi, ( char * ) regs->rsi, regs->rdx );
		case 2:
			return sys_get_register_snapshot ( ( snapshot * ) regs->rdi );
		case 3:
			return sys_beep ( regs->rdi, regs->rsi );
		case 4:
			return sys_set_font_size ( regs->rdi );
		case 5:
			return sys_clear_screen();
		case 6:
			return sys_put_pixel ( regs->rdi, regs->rsi, ( color * ) regs->rdx );
		case 7:
			return sys_put_rectangle ( regs->rdi, regs->rsi, regs->rdx, regs->rcx, ( color * ) regs->r8 );
		case 8:
			return sys_draw_letter ( regs->rdi, regs->rsi, ( char * ) regs->rdx, ( color * ) regs->rcx, regs->r8 );
		case 9:
			return sys_set_mode ( regs->rdi );
		case 10:
			return sys_get_screen_information ( ( screen_information * ) regs->rdi );
		case 11:
			return sys_ticks_sleep ( regs->rdi );
		case 12:
			return sys_get_time ( ( time_struct * ) regs->rdi );
		case 13:
			return ( int64_t ) sys_malloc ( regs->rdi );
		case 14:
			return sys_free_wrapper ( ( void * ) regs->rdi );
		case 15:
			return sys_get_pid();
		case 16:
			return sys_create_process ( ( main_function ) regs->rdi, ( priority_t ) regs->rsi, ( char ** ) regs->rdx, regs->rcx, (fd_t *) regs->r8);
		case 17:
			return sys_block ( regs->rdi );
		case 18:
			return sys_unblock ( regs->rdi );
		case 19:
			return sys_yield();
		case 20:
			return sys_nice( (int64_t) regs->rdi, regs->rsi);
		case 21:
			return sys_kill( (int64_t) regs->rdi );
		case 22:
			return sys_wait( (int64_t) regs->rdi , (int64_t * ) regs->rsi);
		case 23:
			return sys_sem_open( (int64_t) regs->rdi, regs->rsi);
		case 24:
			return sys_sem_wait( (int64_t) regs->rdi);
		case 25:
			return sys_sem_post( (int64_t) regs->rdi);
		case 26:
			return sys_sem_close( (int64_t) regs->rdi);
		case 27: 
			return (int64_t) sys_ps();		
		case 28: 
			return sys_free_ps_wrapper((process_info_list *) regs->rdi);
		case 29:
			int8_t ans = sys_get_status((pid_t) regs->rdi);
			return (int64_t) ans;
		case 30:
			return sys_pipe_open(regs->rdi, (pipe_mode_t) regs->rsi);
		case 31:
			return sys_pipe_open_free((pipe_mode_t)regs->rdi);
		case 32:
			return sys_pipe_read(regs->rdi, (uint16_t *) regs->rsi, regs->rdx);
		case 33:
			return sys_pipe_write(regs->rdi, (uint16_t *) regs->rsi, regs->rdx);
		case 34:
			return sys_pipe_close(regs->rdi);
		case 35:
			return sys_pipe_reserve();
		case 36:
			return sys_sem_open_get_id(regs->rdi); 
		case 37: 
			return sys_get_my_fds(regs->rdi);
		default:
			return NOT_VALID_SYS_ID;
	}
}

int64_t sys_get_my_fds(fd_t fds[CANT_FDS]){
	PCB * pcb = get_running();
	if(pcb == NULL || fds == NULL){
		return -1;
	} 
	for(int i=0; i<CANT_FDS; i++){
		fds[i] = pcb->fds[i];
	}
	return 0;
}

int64_t sys_pipe_open(int64_t id, pipe_mode_t mode){
	if(id < 3){
		return -1;
	}
	return pipe_open(id - 3, mode);
}

int64_t sys_pipe_open_free(pipe_mode_t mode){
	return pipe_open_free(mode) + 3 ;
}

int64_t sys_pipe_read(int64_t id, uint16_t * buffer, uint64_t amount){
	if(id < 3){
		return -1;
	}
	return pipe_read(id - 3, buffer, amount);
}
int64_t sys_pipe_write(int64_t id, uint16_t * buffer, uint64_t amount){
	if(id < 3){
		return -1;
	}
	return pipe_write(id - 3, buffer, amount);
}
int64_t sys_pipe_close(int64_t id){
	if(id < 3){
		return -1;
	}
	return pipe_close(id - 3);
}
int64_t sys_pipe_reserve(){
	return pipe_reserve() + 3;
}


int64_t sys_read (  uint16_t * buffer, uint64_t amount )
{
	fd_t fd = get_running()->fds[STDIN];
	if(fd == STDIN) {
		return stdin_read(buffer, amount);
	}
	return sys_pipe_read(fd,buffer, amount);
}


//modo texto:
int64_t sys_write ( uint64_t fd, const char * buffer, uint64_t amount )
{
	if(fd != STDOUT && fd != STDERR){
		return -1;
	}

	fd_t actual_fd = get_running()->fds[fd];
	// if(actual_fd == -1){
	// 	return -1;
	// }
	if(actual_fd == STDOUT || actual_fd == STDERR){
		return vdriver_text_write ( fd, buffer, amount );
	}

	return sys_pipe_write(actual_fd,buffer, amount);
}

//modo texto:
int64_t sys_set_font_size ( uint64_t size )
{
	return vdriver_text_set_font_size ( size );
}

int64_t sys_beep ( uint32_t freq, uint32_t time )
{
	beep ( freq, time );
	return 0;
}

//modo video:
int64_t sys_put_rectangle ( uint64_t x, uint64_t y, uint64_t width, uint64_t height, color * color )
{
	return vdriver_video_draw_rectangle ( x, y, width, height, *color );
}

//modo video:
int64_t sys_draw_letter ( uint64_t x, uint64_t y, char * letter, color * color, uint64_t font_size )
{
	return vdriver_video_draw_font ( x, y, *letter, *color, font_size );
}

//modo video:
int64_t sys_put_pixel ( uint64_t x, uint64_t y, color * color )
{
	return vdriver_video_draw_pixel ( x, y, *color );
}

int64_t sys_get_screen_information ( screen_information * screen_information )
{
	return vdriver_get_screen_information ( screen_information );
}

int64_t sys_set_mode ( uint64_t mode )
{
	return vdriver_set_mode ( mode, ( color ) {
		0, 0, 0
	} );
}


int64_t sys_get_register_snapshot ( snapshot * snapshot )
{

	if ( !regs_shot_available ) {
		return -1;
	}

	snapshot->rax = regs_shot[0];
	snapshot->rbx = regs_shot[1];
	snapshot->rcx = regs_shot[2];
	snapshot->rdx = regs_shot[3];
	snapshot->rsi = regs_shot[4];
	snapshot->rdi = regs_shot[5];
	snapshot->rbp = regs_shot[6];
	snapshot->rsp = regs_shot[7];
	snapshot->r8 = regs_shot[8];
	snapshot->r9 = regs_shot[9];
	snapshot->r10 = regs_shot[10];
	snapshot->r11 = regs_shot[11];
	snapshot->r12 = regs_shot[12];
	snapshot->r13 = regs_shot[13];
	snapshot->r14 = regs_shot[14];
	snapshot->r15 = regs_shot[15];
	snapshot->rip = regs_shot[16];
	return 0;
}


int64_t sys_clear_screen()
{
	return vdriver_clear_screen ( ( color ) {
		0, 0, 0
	} );
}

int64_t sys_ticks_sleep ( uint64_t ns )
{
	return ticks_sleep ( ns );
}



int64_t sys_get_time ( time_struct * time )
{
	time->seconds = get_rtc_seconds();
	time->minutes =  get_rtc_minutes();
	time->hour =  get_rtc_hours();
	time->day = get_rtc_day_of_month();
	time->month = get_rtc_month();
	time->year = get_rtc_year();
	return 0;
}




//added for tp2_so:


void * sys_malloc ( uint64_t size )
{
	return my_malloc ( size );
}
static int64_t sys_free_wrapper ( void * p )
{
	my_free ( p );
	return 0;
}



int64_t sys_create_process (main_function rip, priority_t priority, char ** argv, uint64_t argc, fd_t fds[])
{
	// fd_t fds[3] = {STDOUT, STDERR, STDIN};
	return (int64_t) new_process( rip, priority, 1, argv, argc, fds);
}

int64_t sys_get_pid()
{
	return get_pid();
}

int64_t sys_nice ( pid_t pid, uint64_t new_prio )
{
	return nice(pid, new_prio);
}

int64_t sys_kill ( pid_t pid )
{
	return kill_process(pid);
}

int64_t sys_block ( pid_t pid )
{
	return block_arbitrary ( pid );
}

int64_t sys_unblock ( pid_t pid )
{
	return unblock_arbitrary ( pid );
}

int64_t sys_yield()
{
	scheduler_yield();
	return 0;
}

int64_t sys_wait ( pid_t pid, int64_t * ret )
{
	return wait(pid, ret);
}

int64_t sys_sem_open ( int64_t sem_id, int value )
{
	return my_sem_open(sem_id, value, 0);
}

int64_t sys_sem_wait ( int64_t sem_id )
{
	return my_sem_wait(sem_id, 0);
}

int64_t sys_sem_post ( int64_t sem_id )
{
	return my_sem_post(sem_id, 0);
}

int64_t sys_sem_open_get_id(int value){
	return my_sem_open_get_id(value);
}

int64_t sys_sem_close ( int64_t sem_id )
{
	return my_sem_close(sem_id, 0);
}

process_info_list * sys_ps ()
{
	return ps();
}

static int64_t sys_free_ps_wrapper(process_info_list * ps){
	free_ps(ps);
	return 0;
}

int8_t sys_get_status(pid_t pid){
	return get_status(pid);
}


