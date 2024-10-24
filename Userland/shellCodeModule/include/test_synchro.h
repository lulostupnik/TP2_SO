
#ifndef _TEST_SYNCHRO_H
#define _TEST_SYNCHRO_H

#include <stdint.h>
#include <stdio.h>
#include <syscall.h>
#include <test_util.h>
#include <libc.h>

uint64_t my_process_inc(uint64_t argc, char *argv[]);


#define SEM_ID 3
#define TOTAL_PAIR_PROCESSES 2

#endif
