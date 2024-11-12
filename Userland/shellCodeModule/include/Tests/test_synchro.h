
#ifndef _TEST_SYNCHRO_H
#define _TEST_SYNCHRO_H

#include <syscall.h>
#include <test_util.h>
#include <libc.h>

uint64_t my_process_inc ( char *argv[], uint64_t argc );
int64_t test_sync ( char *argv[], uint64_t argc );

#define SEM_ID 80
#define TOTAL_PAIR_PROCESSES 2

#endif
