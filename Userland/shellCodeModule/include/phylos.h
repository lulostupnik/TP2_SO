#ifndef _PHYLOS_H
#define _PHYLOS_H

#include <libc.h>
#include <shared_libc.h>
#include <shared_defs.h>
#include <driversInformation.h>
#include <shell.h>

#define MAX_PHILOS 20
#define MIN_PHILOS 5
#define HUNGRY  2
#define EATING  1
#define THINKING  0
#define CONTINUE 1
#define FINISH 0

int phylos(char ** argv, int argc);


#endif