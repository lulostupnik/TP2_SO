#ifndef CMD_INTERPRETER_H
#define CMD_INTERPRETER_H

#include <libc.h>
#include <exceptions.h>
#include <test_prio.h>
#include <test_proc.h>
#include <test_mm.h>
#include <test_synchro.h>
#include <phylos.h>
#include <programs.h>

#define MAX_COMMAND_SIZE 100
#define MAX_ZOOM 3
#define MIN_ZOOM 1
#define MAX_MODULES 19
#define NUM_TESTS 4
#define NUM_BUILT_INS 8
#define MAX_ARGS 10
#define MAX_ARGS_SIZE 20
//Constants for the shell

#define WELCOME "\nBienvenido a BarriOS!"
#define PROMPT "BarriOS>User> "
#define MAX_FONT_SIZE 3
#define MIN_FONT_SIZE 1

#define BUILTIN_BACKGROUND_ERROR 2

typedef struct module
{
    char *name;
	char *args; 
    char *desc;
    void (*function)(char **args, uint64_t argc);
    uint8_t is_built_in;
} module;

int main();




#endif /* CMD_INTERPRETER_H */