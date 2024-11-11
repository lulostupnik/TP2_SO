
#ifndef TEST_MM_H
#define TEST_MM_H

#include <stdint.h>
#include <test_util.h>
#include <syscall.h>
#include <shared_libc.h>
#include <libc.h>

#define MAX_BLOCKS 20
int64_t test_mm ( char * argv[],  uint64_t argc );


#endif
