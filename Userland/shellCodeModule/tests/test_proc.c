// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <test_proc.h>

enum State { RUNNING,
             BLOCKED,
             KILLED
           };

typedef struct p_rq {
	int32_t pid;
	enum State state;
} p_rq;

int64_t test_processes( char * argv[], uint64_t argc)
{
	uint64_t rq;
	uint64_t alive = 0;
	uint64_t action;
	int64_t max_processes;

	if (argc != 2 || (max_processes = satoi(argv[1])) < 0) {
		libc_fprintf(STDERR, "Usage: test_processes <max_processes>\n");
		return -1;
	}

	if (max_processes > 20 || max_processes == 0) {
		libc_fprintf(STDERR, "max_processes must be between 1 and 20\n");
	}

	p_rq p_rqs[max_processes];

	while (1) {
		for (rq = 0; rq < max_processes; rq++) {
			p_rqs[rq].pid = libc_create_process((main_function)endless_loop, 0, NULL, 0);
			if (p_rqs[rq].pid < 0) {
				libc_fprintf(STDERR, "test_processes: ERROR creating process\n");
				return -1;
			} else {
				p_rqs[rq].state = RUNNING;
				alive++;
			}
		}

		while (alive > 0) {

			for (rq = 0; rq < max_processes; rq++) {
				action = get_uniform(100) % 2;

				switch (action) {
					case 0:
						if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == BLOCKED) {
							if (libc_kill(p_rqs[rq].pid) == -1) {
								libc_fprintf(STDERR, "test_processes: ERROR killing process\n");
								return -1;
							}
							p_rqs[rq].state = KILLED;
							alive--;

						}
						break;

					case 1:
						if (p_rqs[rq].state == RUNNING) {
							if (libc_block(p_rqs[rq].pid) == -1) {
								libc_fprintf(STDERR, "test_processes: ERROR blocking process\n");
								return -1;
							}
							p_rqs[rq].state = BLOCKED;
						}
						break;
				}
			}

			for (rq = 0; rq < max_processes; rq++)
				if (p_rqs[rq].state == BLOCKED && get_uniform(100) % 2) {
					if (libc_unblock(p_rqs[rq].pid) == -1) {
						libc_fprintf(STDERR, "test_processes: ERROR unblocking process\n");
						return -1;
					}
					p_rqs[rq].state = RUNNING;
				}
		}
	}
}
