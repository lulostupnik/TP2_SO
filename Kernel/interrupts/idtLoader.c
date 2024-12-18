// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com


#include <idt_loader.h>


#pragma pack(push)
#pragma pack (1)


typedef struct {
	uint16_t offset_l, selector;
	uint8_t cero, access;
	uint16_t offset_m;
	uint32_t offset_h, other_cero;
} DESCR_INT;

#pragma pack(pop)



DESCR_INT * idt = ( DESCR_INT * ) 0;

static void setup_idt_entry ( int index, uint64_t offset );

void load_idt()
{

	_cli();

	setup_idt_entry ( 0x00, ( uint64_t ) &_exception0Handler );  // Division by 0 exception
	setup_idt_entry ( 0x06, ( uint64_t ) &_exception6Handler );  // Operation code exception
	setup_idt_entry ( 0x0D, ( uint64_t ) &_exception13Handler ); // General Protection exception


	setup_idt_entry ( 0x20, ( uint64_t ) &_irq00Handler ); // Timer tick interruption
	setup_idt_entry ( 0x21, ( uint64_t ) &_irq01Handler ); // Keyboard interruption
	setup_idt_entry ( 0x80, ( uint64_t ) &_irq80Handler ); // Syscall interruption

	pic_master_mask ( DEFAULT_MASTER_MASK );
	pic_slave_mask ( DEFAULT_SLAVE_MASK );

}

static void setup_idt_entry ( int index, uint64_t offset )
{
	idt[index].selector = 0x08;
	idt[index].offset_l = offset & 0xFFFF;
	idt[index].offset_m = ( offset >> 16 ) & 0xFFFF;
	idt[index].offset_h = ( offset >> 32 ) & 0xFFFFFFFF;
	idt[index].access = ACS_INT;
	idt[index].cero = 0;
	idt[index].other_cero = ( uint64_t ) 0;
}
