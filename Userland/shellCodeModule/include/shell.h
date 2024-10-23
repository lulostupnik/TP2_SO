#ifndef CMD_INTERPRETER_H
#define CMD_INTERPRETER_H

#include <libc.h>
#include <eliminator.h>
#include <exceptions.h>
#include <ipod.h>
#include <test_prio.h>
#include <test_proc.h>
#include <test_mm.h>

#define MAX_COMMAND_SIZE 100
#define MAX_ZOOM 3
#define MIN_ZOOM 1
#define MAX_MODULES 14
#define MAX_ARGS 10
#define MAX_ARGS_SIZE 20
//Constants for the shell

#define WELCOME "Bienvenido a BarriOS!\n"
#define PROMPT "BarriOS>User> "
#define MAX_FONT_SIZE 3
#define MIN_FONT_SIZE 1

typedef struct module
{
	char * name;
	void ( *function )(char ** args, uint64_t argc);
	uint8_t is_built_in;
} module;



int main();




#endif /* CMD_INTERPRETER_H */