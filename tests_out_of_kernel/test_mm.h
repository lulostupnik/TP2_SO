#ifndef TEST_MM_H
#define TEST_MM_H

#include "syscall.h"
#include "test_util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mm.h"

#define MAX_BLOCKS BLOCK_COUNT

uint64_t test_mm ( uint64_t argc, char *argv[] );

#endif