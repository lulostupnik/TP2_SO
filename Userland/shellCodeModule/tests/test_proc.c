// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <test_proc.h>

enum State { STATE_RUNNING,
             STATE_BLOCKED,
             STATE_KILLED
           };

typedef struct p_rq {
	pid_t pid;
	enum State state;
} p_rq;


int64_t test_processes( char * argv[], uint64_t argc)
{
	uint64_t rq;
	uint64_t alive = 0;
	uint64_t action;
	int64_t max_processes;
	int64_t satoi_flag;
	if (argc != 2 || (max_processes = libc_satoi(argv[1], &satoi_flag)) < 0 || !satoi_flag) {
		libc_fprintf(STDERR, "Usage: testproc <max_processes>\n");
		return -1;	
	}

	if (max_processes > 20 || max_processes == 0) {
		libc_fprintf(STDERR, "<max_processes> must be between 1 and 20\n");
		return -1;	
	}

	p_rq p_rqs[max_processes];

	while (1) {
		for (rq = 0; rq < max_processes; rq++) {
			fd_t fds[] = {STDOUT, STDERR, -1};
			char * argv[] = {"test_proc_aux"};
			p_rqs[rq].pid = libc_create_process((main_function)endless_loop, 0, argv, 1, fds);
			if (p_rqs[rq].pid < 0) {
				libc_fprintf(STDERR, "ERROR creating process\n");
				return -1;
			} else {
				p_rqs[rq].state = STATE_RUNNING;
				alive++;
			}
		}

		while (alive > 0) {

			for (rq = 0; rq < max_processes; rq++) {
				action = get_uniform(100) % 2;

				switch (action) {
					case 0:
						if (p_rqs[rq].state == STATE_RUNNING || p_rqs[rq].state == STATE_BLOCKED) {
							if (libc_kill(p_rqs[rq].pid) == -1) {
								libc_fprintf(STDERR, "ERROR killing process\n");
								return -1;
							}
							p_rqs[rq].state = STATE_KILLED;
							alive--;

						}
						break;

					case 1:
						if (p_rqs[rq].state == STATE_RUNNING) {
							if (libc_block(p_rqs[rq].pid) == -1) {
								libc_fprintf(STDERR, "ERROR blocking process\n");
								return -1;
							}
							p_rqs[rq].state = STATE_BLOCKED;
						}
						break;
				}
			}

			for (rq = 0; rq < max_processes; rq++)
				if (p_rqs[rq].state == STATE_BLOCKED && get_uniform(100) % 2) {
					if (libc_unblock(p_rqs[rq].pid) == -1) {
						libc_fprintf(STDERR, "ERROR unblocking process\n");
						return -1;
					}
					p_rqs[rq].state = STATE_RUNNING;
				}
		}
	}
}
