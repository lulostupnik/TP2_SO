#include <libc.h>
#include <eliminator.h>
#include <exceptions.h>
#include <ipod.h>
#include <test_mm_module.h>
#include <test_prio.h>
#include <test_proc.h>

#ifndef CMD_INTERPRETER_H
#define CMD_INTERPRETER_H
#define MAX_COMMAND_SIZE 100
#define MAX_ZOOM 3
#define MIN_ZOOM 1
#define MAX_MODULES 13
//Constants for the shell

#define WELCOME "Bienvenido a BarriOS!\n"
#define PROMPT "BarriOS>User> "
#define MAX_FONT_SIZE 3
#define MIN_FONT_SIZE 1

//Type Definitions
typedef struct module
{
	char *name;
	void ( *function ) ();
	uint8_t is_built_in;
	char ** argv;
	uint64_t argc;
	uint64_t priority; //@todo cambiar por el enum
} module;




// Function prototypes
int initializeShell();

void interpret();

// Function to zoom in
void zoomIn();

// Function to zoom out
void zoomOut();

void showcurrentTime();

void getRegs();

void clear();



// --------------- TP2:



#endif /* CMD_INTERPRETER_H */