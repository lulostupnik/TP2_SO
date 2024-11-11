#ifndef _PHYLOS_H
#define _PHYLOS_H

#include <libc.h>
#include <shared_libc.h>
#include <shared_defs.h>
#include <drivers_information.h>
#include <shell.h>

#define MAX_PHILOS 15
#define MIN_PHILOS 5
#define EATING  1
#define THINKING  0
#define CONTINUE 1
#define FINISH 0

#define SLEEP_CONSTANT 3
#define THINK_CONSTANT 1
#define GET_UNIFORM_CONSTANT 3
int64_t phylo ( char ** argv, int argc );

#endif