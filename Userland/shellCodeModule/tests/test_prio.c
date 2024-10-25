// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <test_prio.h>

int64_t prio[TOTAL_PROCESSES] = {LOW, MEDIUM, HIGH};

void test_prio()
{
	pid_t pids[TOTAL_PROCESSES];
	char * str = WAIT_STRING;
	char * argv[] = {str};
	uint64_t i;

	libc_printf("\n");
	for (i = 0; i < TOTAL_PROCESSES; i++) {
		pids[i] = libc_create_process((main_function)endless_loop_print_main, 0, argv, 1);
		if (pids[i] < 0) {
			libc_fprintf ( STDERR, "Failed to create process number %d\n", i + 1 );
		}
	}
	bussy_wait(WAIT);
	libc_printf("\nCHANGING PRIORITIES ...\n");

	for (i = 0; i < TOTAL_PROCESSES; i++) {
		libc_nice(pids[i], prio[i]);
		libc_printf("Pid %d with priority %s\n", pids[i], prio[i] == 0 ? "LOW" : (prio[i] == 1 ? "MEDIUM" : "HIGH"));
	}
	bussy_wait(WAIT);
	libc_printf("\nBLOCKING...\n");

	for (i = 0; i < TOTAL_PROCESSES; i++) {
		libc_block(pids[i]);
	}

	libc_printf("CHANGING PRIORITIES WHILE BLOCKED (TO MEDIUM)...\n");

	for (i = 0; i < TOTAL_PROCESSES; i++) {
		libc_nice(pids[i], MEDIUM);
	}
	libc_printf("UNBLOCKING...\n");

	for (i = 0; i < TOTAL_PROCESSES; i++) {
		libc_unblock(pids[i]);
	}
	bussy_wait(WAIT);
	libc_printf("\nKILLING...\n");

	for (i = 0; i < TOTAL_PROCESSES; i++) {
		libc_kill(pids[i]);
	}
	libc_kill(libc_get_pid());
	return;
}