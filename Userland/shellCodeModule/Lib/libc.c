// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <libc.h>


char libc_get_char()
{
    int64_t ans = 0;
	uint8_t c = 0;
	ans = sys_read( &c, 1);  
    return ans <= 0 ? 0 : (char) c;
}

void libc_put_char ( char c )
{
	sys_write ( STDOUT, (uint8_t *) &c, 1 );
}

int64_t libc_beep ( uint64_t frequency, uint64_t duration )
{
	return sys_beep ( frequency, duration );
}


int64_t libc_clear_screen()
{
	return sys_clear_screen();
}


int64_t libc_set_font_size ( uint64_t size )
{
	return sys_set_font_size ( size );
}



char * libc_num_to_string(uint64_t num, uint64_t base, char *buffer, size_t buffer_size) {
    if (base < 2 || base > 16 || buffer_size < 2) {
        return NULL;  
    }

    char *ptr = &buffer[buffer_size - 1];  
    *ptr = '\0';  

    do {
        if (ptr == buffer) {
            return NULL;  
        }
        *--ptr = "0123456789ABCDEF"[num % base];
        num /= base;
    } while (num != 0);

    return ptr;  
}

int64_t libc_puts (  char * str )
{
	return sys_write ( STDOUT, (uint8_t * )str, shared_libc_strlen ( str ) );
}

int64_t libc_fputc(char c, uint64_t fd) {
	if(c < 0){
		return -1;
	}
    return sys_write(fd, (uint8_t * ) &c, 1) == -1 ? -1 : 1;
}

#define BUFF_SIZE 64

static int64_t libc_vfprintf(uint64_t fd, char *fmt, va_list argv) {
    uint64_t flag = 0;
    int64_t written = 0;
    char buffer[BUFF_SIZE];

    for (uint64_t i = 0; fmt[i] != '\0'; i++) {
        if (fmt[i] == '%' && !flag) {
            flag = 1;
            i++;
        }

        if (!flag) {
            if (libc_fputc(fmt[i], fd) == -1) {
                return -1;
            }
            flag = 0;
            written++;
            continue;
        }

        switch (fmt[i]) {
            case 'c':
                if (libc_fputc(va_arg(argv, int), fd) == -1) {
                    return -1;
                }
                written++;
                break;
            case 'd':
                if ((written += libc_vfprintf(fd, libc_num_to_string(va_arg(argv, uint64_t), 10, buffer, BUFF_SIZE), argv)) == -1) {
                    return -1;
                }
                break;
            case 'x':
                if (libc_vfprintf(fd, "0x", argv) == -1 ||
                    (written += libc_vfprintf(fd, libc_num_to_string(va_arg(argv, uint64_t), 16, buffer, BUFF_SIZE), argv)) == -1) {
                    return -1;
                }
                break;
            case 's':
                if ((written += libc_vfprintf(fd, va_arg(argv, char *), argv)) == -1) {
                    return -1;
                }
                break;
            case '%':
                if (libc_fputc('%', fd) == -1) {
                    return -1;
                }
                written++;
                break;
            default:
                return -1;
        }
        flag = 0;
    }

    return written;
}

int64_t libc_fprintf(uint64_t fd, char *fmt, ...) {
    va_list argv;
    va_start(argv, fmt);

    int64_t out = libc_vfprintf(fd, fmt, argv);

    va_end(argv);
    return out;
}

int64_t libc_printf( char *fmt, ...) {
    va_list argv;
    va_start(argv, fmt);

    int64_t out = libc_vfprintf(STDOUT, fmt, argv);

    va_end(argv);
    return out;
}


char * libc_gets ( char * buffer, int n )
{
	int c;
	int i = 0;

	while ( ( c = libc_get_char() ) != '\n'  ) {
		if ( c == '\b' && i > 0 && c != 0) {
			libc_put_char ( c );
			i--;
		}
		if ( c != '\b' && i < n - 1 && c != 0) {
			libc_put_char ( c );
			buffer[i++] = ( char ) c;
		}
	}
	libc_put_char ( '\n' );
	buffer[i] = '\0';

	return buffer;
}


int64_t libc_strcmp (  char * str1,  char * str2 )
{
	while ( *str1 && ( *str1 == *str2 ) ) {
		str1++;
		str2++;
	}
	return * ( unsigned char * ) str1 - * ( unsigned char * ) str2;
}

int64_t libc_strnocasecmp( char *str1,  char *str2) {
    while (*str1 && *str2) {
        char c1 = *str1;
        char c2 = *str2;

        if (c1 >= 'A' && c1 <= 'Z') {
            c1 = c1 + ('a' - 'A');
        }
        if (c2 >= 'A' && c2 <= 'Z') {
            c2 = c2 + ('a' - 'A');
        }

        if (c1 != c2) {
            return (int64_t)(unsigned char)c1 - (unsigned char)c2;
        }

        str1++;
        str2++;
    }

    return (int64_t)(unsigned char)*str1 - (unsigned char)*str2;
}


int64_t libc_enter_video_mode()
{
	return sys_set_mode ( VIDEO_MODE );
}


int64_t libc_enter_text_mode()
{
	return sys_set_mode ( TEXT_MODE );
}

int64_t libc_draw_pixel ( uint64_t x, uint64_t y, color color )
{
	return sys_put_pixel ( x, y, &color );
}

int64_t libc_draw_rectangle ( uint64_t x, uint64_t y, uint64_t width, uint64_t height, color color )
{
	return sys_put_rectangle ( x, y, width, height, &color );
}

int64_t libc_draw_letter ( uint64_t x, uint64_t y, char letter, color color, uint64_t font_size )
{
	return sys_draw_letter ( x, y, &letter, &color, font_size );
}

void libc_print_register_snapshot()
{
	snapshot snap;
	if ( sys_get_register_snapshot ( &snap ) == -1 ) {
		libc_fprintf ( STDERR, "Error: No register snapshot available. Press F1 to take a snapshot.\n" );
		return;
	}

	libc_puts ( "Register snapshot:\n" );
	libc_printf ( "rax: %x\n", snap.rax );
	libc_printf ( "rbx: %x\n", snap.rbx );
	libc_printf ( "rcx: %x\n", snap.rcx );
	libc_printf ( "rdx: %x\n", snap.rdx );
	libc_printf ( "rsi: %x\n", snap.rsi );
	libc_printf ( "rdi: %x\n", snap.rdi );
	libc_printf ( "rbp: %x\n", snap.rbp );
	libc_printf ( "rsp: %x\n", snap.rsp );
	libc_printf ( "r8:  %x\n", snap.r8 );
	libc_printf ( "r9:  %x\n", snap.r9 );
	libc_printf ( "r10: %x\n", snap.r10 );
	libc_printf ( "r11: %x\n", snap.r11 );
	libc_printf ( "r12: %x\n", snap.r12 );
	libc_printf ( "r13: %x\n", snap.r13 );
	libc_printf ( "r14: %x\n", snap.r14 );
	libc_printf ( "r15: %x\n", snap.r15 );
	libc_printf ( "rIP: %x\n", snap.rip );
}

//----------------------------- TP2 ---------------------------------- //


void * libc_malloc ( uint64_t size )
{
	return sys_malloc ( size );
}

void libc_free ( void * p )
{
	return sys_free ( p );
}

pid_t libc_create_process( main_function rip, uint64_t priority, char ** argv, uint64_t argc, fd_t fds[])
{
	return (pid_t) sys_create_process(rip, priority, argv, argc, fds);
}
int64_t libc_get_time ( time_struct * time ){
	return sys_get_time ( time );
}

pid_t libc_get_pid()
{
	return (pid_t) sys_get_pid();
}

int64_t libc_kill ( pid_t pid )
{
	return sys_kill(pid);
}
int64_t libc_block ( pid_t pid )
{
	return sys_block(pid);
}
int64_t libc_nice(pid_t pid, uint64_t newPrio )
{
	return sys_nice(pid, newPrio);
}

int64_t libc_yield()
{
	return sys_yield();
}
int64_t libc_unblock ( pid_t pid )
{
	return sys_unblock(pid);
}

pid_t libc_wait ( pid_t pid, int64_t * ret){
	return sys_wait(pid, ret);
}

int64_t libc_sem_open ( int64_t sem_id, int64_t value ){
	return sys_sem_open(sem_id, value);
}

int64_t libc_sem_wait ( int64_t sem_id ){
	return sys_sem_wait(sem_id);
}

int64_t libc_sem_post ( int64_t sem_id ){
	return sys_sem_post(sem_id);
}

int64_t libc_sem_close ( int64_t sem_id ){
	return sys_sem_close(sem_id);
}
int64_t libc_sem_open_get_id( int64_t value ){
	return sys_sem_open_get_id(value);
}

int8_t libc_get_status(pid_t pid){
	return sys_get_status(pid);
}
int64_t libc_pipe_open(int64_t id, pipe_mode_t mode){
	return sys_pipe_open(id, mode);
}
int64_t libc_pipe_open_free(pipe_mode_t mode){
	return sys_pipe_open_free(mode);
}
int64_t libc_pipe_read(int64_t id, uint8_t * buffer, uint64_t amount){
	return sys_pipe_read( id,  buffer,  amount);
}
int64_t libc_pipe_write(int64_t id, uint8_t * buffer, uint64_t amount){
	return sys_pipe_write( id,  buffer, amount);
}
int64_t libc_pipe_close(int64_t id){
	return sys_pipe_close(id);
}
int64_t libc_get_my_fds(fd_t fds[CANT_FDS]){
	return sys_get_my_fds(fds);
}

int64_t libc_ticks_sleep ( uint64_t ticks ){
	return sys_ticks_sleep(ticks);
}


int64_t libc_satoi ( char * str, int64_t * flag )
{
	uint64_t i = 0;
	int64_t res = 0;
	int8_t sign = 1;

	if ( !str ){
		if(flag){
			*flag = 0;
		}
		return 0;}

	if ( str[i] == '-' ) {
		i++;
		sign = -1;
	}

	for ( ; str[i] != '\0'; ++i ) {
		if ( str[i] < '0' || str[i] > '9' ){
			if(flag){
				*flag = 0;
			}
			return 0;
		}
			
		res = res * 10 + str[i] - '0';
	}
	if(flag){
		*flag = 1;
	}
	return res * sign;
}

