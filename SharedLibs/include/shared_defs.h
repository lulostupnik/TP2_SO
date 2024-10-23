#ifndef _SHARED_DEFS_H
#define _SHARED_DEFS_H

typedef struct {
	uint64_t rax;
	uint64_t rbx;
	uint64_t rcx;
	uint64_t rdx;
	uint64_t rsi;
	uint64_t rdi;
	uint64_t rbp;
	uint64_t rsp;
	uint64_t r8;
	uint64_t r9;
	uint64_t r10;
	uint64_t r11;
	uint64_t r12;
	uint64_t r13;
	uint64_t r14;
	uint64_t r15;
	uint64_t rip;
} snapshot;

typedef struct {
	uint64_t r15, r14, r13, r12, r11, r10, r9, r8, rsi, rdi, rbp, rdx, rcx, rbx, rax;
} stack_registers;

typedef struct {
	int64_t width;
	int64_t height;
} screen_information;


typedef struct time {
	uint8_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minutes;
	uint8_t seconds;
} time_struct;


typedef int64_t pid_t;

typedef enum { LOW = 0, MEDIUM, HIGH } priority_t;

#define FREE 0  
#define READY 1
#define BLOCKED 2
#define ZOMBIE 3

typedef int (*main_function)(char ** argv, uint64_t argc);


#endif