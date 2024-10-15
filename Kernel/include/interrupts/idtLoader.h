#ifndef _IDTLOADER_H_
#define _IDTLOADER_H_

#include <stdint.h>
#include <defs.h>
#include <interrupts.h>

#define DEFAULT_MASTER_MASK 0xFC
#define DEFAULT_SLAVE_MASK 0xFF


void load_idt();


#endif
