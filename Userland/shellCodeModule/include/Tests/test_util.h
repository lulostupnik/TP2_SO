
#ifndef TEST_UTIL_H
#define TEST_UTIL_H
#include <stdint.h>
#include <syscall.h>
#include <libc.h>

uint32_t get_u_int();
uint32_t get_uniform ( uint32_t max );
uint8_t memcheck ( void * start, uint8_t value, uint32_t size );
void bussy_wait ( uint64_t n );
void endless_loop();
void endless_loop_print ( uint64_t wait );
void endless_loop_print_main ( char ** argv, uint64_t argc );

#endif