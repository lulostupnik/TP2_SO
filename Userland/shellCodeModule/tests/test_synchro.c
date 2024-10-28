#include <test_synchro.h>

int64_t global; // shared memory

void slowInc(int64_t *p, int64_t inc) {
  uint64_t aux = *p;
  libc_yield(); // This makes the race condition highly probable
  aux += inc;
  *p = aux;
}

uint64_t my_process_inc(char *argv[], uint64_t argc) {
  uint64_t n;
  int8_t inc;
  int8_t use_sem;

  if (argc != 4){
    libc_printf("Usage: process_name_inc <n> <inc> <use_sem>\n");
    return -1;
  }

  if ((n = satoi(argv[1])) <= 0)
    return -1;
  if ((inc = satoi(argv[2])) == 0)
    return -1;
  if ((use_sem = satoi(argv[3])) < 0)
    return -1;

  if (use_sem){
    int64_t open_attempt = libc_sem_open(SEM_ID, 1);
    if (open_attempt == -1) {
      libc_fprintf(STDERR, "test_sync: ERROR opening semaphore\n");
      return -1;
    } 
  }
  uint64_t i;
  for (i = 0; i < n; i++) {
    if (use_sem){
      if(libc_sem_wait(SEM_ID) == -1){
        libc_fprintf(STDERR, "test_sync: ERROR waiting semaphore\n");
        return -1;
      }
    }
    slowInc(&global, inc);
    if (use_sem){
      libc_sem_post(SEM_ID);
    }
  }  
  if (use_sem){
    if(libc_sem_close(SEM_ID) == -1){
      libc_fprintf(STDERR, "%d: ERROR closing semaphore\n", libc_get_pid());
    }
  }

  return 0;
}

uint64_t test_sync(char *argv[], uint64_t argc) { //{n, use_sem, 0}
  pid_t pids[2 * TOTAL_PAIR_PROCESSES];

  if (argc != 3){
    libc_fprintf(STDERR, "Usage: test_sync <n> <use_sem>\n");
    return -1;
  }
  char *argvDec[] = {"process_name_dec", argv[1], "-1", argv[2], NULL};
  char *argvInc[] = {"process_name_inc", argv[1], "1", argv[2], NULL};

  global = 0;

  uint64_t i;
  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    pids[i] = libc_create_process((main_function) my_process_inc, 1, argvDec, 4);
    pids[i + TOTAL_PAIR_PROCESSES] = libc_create_process((main_function) my_process_inc, 1, argvInc, 4);
  }
  
  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    if(libc_wait(pids[i], NULL) == -1){
      libc_fprintf(STDOUT, "test_sync: ERROR waiting for process %d\n", pids[i]);
    }
    if(libc_wait(pids[i + TOTAL_PAIR_PROCESSES], NULL) == -1){
      libc_fprintf(STDOUT, "test_sync: ERROR waiting for process %d\n", pids[i + TOTAL_PAIR_PROCESSES]);
    }
  }
  

  libc_fprintf(STDOUT, "Final value: %d\n", global);
  return 0;
}



