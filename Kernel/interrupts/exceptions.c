// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <exceptions.h>


#define ZERO_EXCEPTION_ID 0
#define OPCODE_EXCEPTION_ID 6
#define GENERAL_PROTECTION 13
#define CANT_REGS 18

extern uint64_t exception_regs[CANT_REGS];
//Orden: RAX, RBX, RCX, RDX, RSI, RDI, RBP, RSP, R8, R9, R10, R11, R12, R13, R14, R15, RIP, RFLAGS


static uint8_t * num_to_string ( uint64_t num, uint64_t base )
{
	static uint8_t buffer[64];
	uint8_t * ptr = &buffer[63];
	*ptr = '\0';
	do {
		*--ptr = "0123456789abcdef"[num % base];
		num /= base;
	} while ( num != 0 );
	return ptr;
}

static void print_regs ( uint8_t * message, uint8_t cant_chars_message )
{
	const char * regs[] = {"rax", "rbx", "rcx", "rdx", "rsi", "rdi", "rbp", "rsp", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15", "rip", "rflags"};
	color col = {255, 255, 255};
	vdriver_clear_screen ( col );
	vdriver_set_mode ( TEXT_MODE, col );
	vdriver_text_set_font_size ( 2 );

	vdriver_text_write ( STDERR, ( char * ) message, cant_chars_message );
	uint8_t newline = '\n';
	for ( int i = 0; i < CANT_REGS; i++ ) {
		vdriver_text_write ( STDERR, regs[i], 10 );
		vdriver_text_write ( STDERR, ": 0x", 4 );
		vdriver_text_write ( STDERR, ( char * ) num_to_string ( exception_regs[i], 16 ), 10 );
		vdriver_text_write ( STDERR, "h", 1 );
		vdriver_text_write ( STDERR, ( char * ) &newline, 1 );
	}
}



void exception_dispatcher ( uint64_t exception )
{

	uint8_t * message = ( uint8_t * ) "";
	uint8_t message_cant_chars = 1;
	if ( exception == ZERO_EXCEPTION_ID ) {
		message = ( uint8_t * ) "Zero div exception\n\n";
		message_cant_chars = 20;
	}

	if ( exception == OPCODE_EXCEPTION_ID ) {
		message = ( uint8_t * ) "Invalid opcode\n\n";
		message_cant_chars = 16;
	}
	if ( exception == GENERAL_PROTECTION ) {
		message = ( uint8_t * ) "General Protection\n\n";
		message_cant_chars = 20;
	}



	print_regs ( message, message_cant_chars );
	vdriver_text_write ( STDERR, "\nPress any key to continue", 30 );
	uint16_t buffer;



	pic_master_mask ( 0xfd );                 //solo habilito interrupciones de teclado
	_sti();
	while(!buffer_has_next()){
		_hlt();
	}
	buffer = get_current();
	_cli();
	pic_master_mask ( DEFAULT_MASTER_MASK );  // restores to default

	color c = {0, 0, 0};
	vdriver_clear_screen ( c );
}