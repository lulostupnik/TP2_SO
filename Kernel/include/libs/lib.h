#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void * memcpy ( void * destination, const void * source, uint64_t length );

char * cpu_vendor ( char * result );

int inb ( int port );
void outb ( int port, int value );

void hlt();
#endif