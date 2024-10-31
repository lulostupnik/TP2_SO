// this is a personal academic project. dear pvs-studio, please check it.
// pvs-studio static code analyzer for c, c++ and c#: http://www.viva64.com

#include <syscallsKernel.h>



extern uint64_t regs_shot[17];
extern uint64_t regs_shot_available;


int64_t sys_call_handler ( stack_registers * regs )
{
	switch ( regs->rax ) {
		case 0:
			return sys_read ( regs->rdi, ( uint16_t * ) regs->rsi, regs->rdx );
			break;
		case 1:
			return sys_write ( regs->rdi, ( char * ) regs->rsi, regs->rdx );
			break;
		case 2:
			return sys_get_register_snapshot ( ( snapshot * ) regs->rdi );
			break;
		case 3:
			return sys_beep ( regs->rdi, regs->rsi );
			break;
		case 4:
			return sys_set_font_size ( regs->rdi );
			break;
		case 5:
			return sys_clear_screen();
			break;
		case 6:
			return sys_put_pixel ( regs->rdi, regs->rsi, ( color * ) regs->rdx );
			break;
		case 7:
			return sys_put_rectangle ( regs->rdi, regs->rsi, regs->rdx, regs->rcx, ( color * ) regs->r8 );
			break;
		case 8:
			return sys_draw_letter ( regs->rdi, regs->rsi, ( char * ) regs->rdx, ( color * ) regs->rcx, regs->r8 );
			break;
		case 9:
			return sys_set_mode ( regs->rdi );
			break;
		case 10:
			return sys_get_screen_information ( ( screen_information * ) regs->rdi );
			break;
		case 11:
			return sys_nano_sleep ( regs->rdi );
			break;
		case 12:
			return sys_get_time ( ( time_struct * ) regs->rdi );
			break;
		case 13:
			return ( int64_t ) sys_malloc ( regs->rdi );
			break;
		case 14:
			sys_free ( ( void * ) regs->rdi );
			return 0;
			break;
		case 15:
			return sys_get_pid();
			break;
		case 16:
			return sys_create_process ( ( main_function ) regs->rdi, ( priority_t ) regs->rsi, ( char ** ) regs->rdx, regs->rcx );
			break;
		case 17:
			return sys_block ( regs->rdi );
			break;
		case 18:
			return sys_unblock ( regs->rdi );
		case 19:
			return sys_yield();
			break;
		case 20:
			return sys_nice( (int64_t) regs->rdi, regs->rsi);
			break;
		case 21:
			return sys_kill( (int64_t) regs->rdi );
			break;
		case 22:
			return sys_wait( (int64_t) regs->rdi , (int64_t * ) regs->rsi);
			break;
		case 23:
			return sys_sem_open( (int64_t) regs->rdi, regs->rsi);
			break;
		case 24:
			return sys_sem_wait( (int64_t) regs->rdi);
			break;
		case 25:
			return sys_sem_post( (int64_t) regs->rdi);
			break;
		case 26:
			return sys_sem_close( (int64_t) regs->rdi);
			break;
		case 27: 
			return (int64_t) sys_ps();
			break;
		/*
		case 27: 
			return sys_ps( (process_info_list *) regs->rdi);
			break;
		*/

		default:
			return NOT_VALID_SYS_ID;

	}
}


int64_t sys_read ( uint64_t fd, uint16_t * buffer, uint64_t amount )
{
	uint64_t i = 0;

	while ( i < amount && buffer_has_next() ) {
		buffer[i] = get_current();
		i++;
	}
	return i;
}


//modo texto:
int64_t sys_write ( uint64_t fd, const char * buffer, uint64_t amount )
{
	return vdriver_text_write ( fd, buffer, amount );
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

int64_t sys_nano_sleep ( uint32_t ns )
{
	nano_sleep ( ns );
	return 0;
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
void sys_free ( void * p )
{
	my_free ( p );
}



int64_t sys_create_process (main_function rip, priority_t priority, char ** argv, uint64_t argc)
{
	return (int64_t) new_process( rip, priority, 1, argv, argc );
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
	return my_sem_open(sem_id, value);
}

int64_t sys_sem_wait ( int64_t sem_id )
{
	return my_sem_wait(sem_id);
}

int64_t sys_sem_post ( int64_t sem_id )
{
	return my_sem_post(sem_id);
}

int64_t sys_sem_close ( int64_t sem_id )
{
	return my_sem_close(sem_id);
}

process_info_list * sys_ps ()
{
	return ps();
}

/*
int64_t sys_ps ( process_info_list * list )
{
	return ps(list);
}
*/