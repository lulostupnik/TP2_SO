#ifndef _TEST_PRIO_
#define _TEST_PRIO_

#include <stdint.h>
#include <syscall.h>
#include <test_util.h>  
#include <shell.h>

#define MINOR_WAIT 1000000       // TODO: Change this value to prevent a process from flooding the screen
#define WAIT 1000000000          // TODO: Change this value to make the wait long enough to see theese processes beeing run at least twice
#define WAIT_STRING "30000000"

#define TOTAL_PROCESSES 3

void test_prio();

#endif