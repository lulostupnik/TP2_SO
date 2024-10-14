#include <syscall.h>
#include <test_util.h>

enum State { RUNNING,
             BLOCKED,
             KILLED };

typedef struct P_rq {
  int32_t pid;
  enum State state;
} p_rq;

int64_t test_processes( char *argv[], uint64_t argc) {
  uint64_t rq;
  uint64_t alive = 0;
  uint64_t action;
  uint64_t max_processes;
  uint64_t iteration = 0;
  char *argvAux[] = {0};

  if (argc != 1){
    return -1;
  }

  if ((max_processes = satoi(argv[0])) <= 0){
    return -1;}

  printf("\n%d max processes\n", max_processes);
  printf("%d argv\n", argv);
  printf("%s argv[0]\n", *argv );
  printf("%d argc\n", argc);
  p_rq p_rqs[max_processes];

  while (1) {
    iteration++;
    printf("Iter num %d\n", iteration);
    for (rq = 0; rq < max_processes; rq++) {
      //p_rqs[rq].pid = my_create_process("endless_loop", 0, argvAux);
      p_rqs[rq].pid = my_create_process(endless_loop, 0, NULL, 0);
      if(rq == 0){
        printf("Min pid %d\n",p_rqs[rq].pid );
      }
      //printf("Created process number %d\n", p_rqs[rq].pid);
      if (p_rqs[rq].pid < 0) {
        printf("test_processes: ERROR creating process\n");
        return -1;
      } else {
        p_rqs[rq].state = RUNNING;
        alive++;
      }
    }
    printf("Max PID %d \n", p_rqs[rq-1].pid);

    while (alive > 0) {

      for (rq = 0; rq < max_processes; rq++) {
        action = GetUniform(100) % 2;

        switch (action) {
          case 0:
            if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == BLOCKED) {
              if (my_kill(p_rqs[rq].pid) == -1) {
                printf("test_processes: ERROR killing process\n");
                printf("Last pid was %d\n",p_rqs[rq-1].pid );
                return -1;
              }
              p_rqs[rq].state = KILLED;
              alive--;
           
            }
            break;

          case 1:
            if (p_rqs[rq].state == RUNNING) {
              if (my_block(p_rqs[rq].pid) == -1) {
                printf("test_processes: ERROR blocking process\n");
                return -1;
              }
              p_rqs[rq].state = BLOCKED;
            }
            break;
        }
      }

      for (rq = 0; rq < max_processes; rq++)
        if (p_rqs[rq].state == BLOCKED && GetUniform(100) % 2) {
          if (my_unblock(p_rqs[rq].pid) == -1) {
            printf("test_processes: ERROR unblocking process\n");
            return -1;
          }
          p_rqs[rq].state = RUNNING;
        }
    }
  }
}
