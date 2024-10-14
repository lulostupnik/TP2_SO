#ifndef _TEST_PRIO
#define _TEST_PRIO

#include <stdint.h>
#include <syscall.h>
#include <test_util.h>  

#define MINOR_WAIT 1000000       // TODO: Change this value to prevent a process from flooding the screen
#define WAIT 1000000000          // TODO: Change this value to make the wait long enough to see theese processes beeing run at least twice
#define WAIT_STRING "30000000"
// #define WAIT 100000000      // TODO: Change this value to make the wait long enough to see theese processes beeing run at least twice
// #define WAIT_STRING "100000000"


#define TOTAL_PROCESSES 3
#define LOWEST 0  // TODO: Change as required
#define MEDIUM 1  // TODO: Change as required
#define HIGHEST 2 // TODO: Change as required

void test_prio();

#endif