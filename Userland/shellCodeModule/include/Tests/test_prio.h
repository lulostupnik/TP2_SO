#ifndef _TEST_PRIO_
#define _TEST_PRIO_

#include <stdint.h>
#include <syscall.h>
#include <test_util.h>
#include <shell.h>

#define MINOR_WAIT 1000000
#define WAIT 1000000000
#define WAIT_STRING "30000000"

#define TOTAL_PROCESSES 3

int64_t test_prio();

#endif