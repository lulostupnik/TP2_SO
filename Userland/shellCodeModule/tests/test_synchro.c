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

  // libc_fprintf(STDERR, "%s - pid:%d, n: %s, inc: %s, use_sem: %s\n", argv[0], libc_get_pid(), argv[1], argv[2], argv[3] );

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
    libc_fprintf(STDERR, "%d is trying to open semaphore %d\n", libc_get_pid(), SEM_ID);
    int64_t open_attempt = libc_sem_open(SEM_ID, 1);
    if (open_attempt == -1) {
      // printf("test_sync: ERROR opening semaphore\n");
      libc_fprintf(STDERR, "test_sync: ERROR opening semaphore\n");
      return -1;
    }
    
   // return 0;
  }
  
  uint64_t i;
  for (i = 0; i < n; i++) {
    if (use_sem){
      // libc_fprintf(STDERR, "%d is waiting semaphore %d\n", libc_get_pid(), SEM_ID);
      libc_fprintf(STDERR, "%d is waiting\n", libc_get_pid());
      if(libc_sem_wait(SEM_ID) == -1){
        // printf("test_sync: ERROR waiting semaphore\n");
        libc_fprintf(STDERR, "test_sync: ERROR waiting semaphore\n");
        return -1;
      }
      // libc_fprintf(STDERR, "%d has passed semaphore %d\n", libc_get_pid(), SEM_ID);
      libc_fprintf(STDOUT, "%d is in critical zone\n", libc_get_pid());
    }
    slowInc(&global, inc);
    if (use_sem){
      libc_fprintf(STDERR, "%d is posting\n", libc_get_pid());
      libc_sem_post(SEM_ID);
    }
  }
  libc_fprintf(STDERR, "%d has finished\n", libc_get_pid());

  /*
  if (use_sem)
    libc_sem_close(SEM_ID);
  */

  
// test_synchro.c line 72

if(libc_get_pid() == 5){
    libc_fprintf(STDERR, "Entre en el proceso 5 para debuggear\n");
    // breakpoint acá para debuggear
  
  }

  return 0;
}

uint64_t test_sync(char *argv[], uint64_t argc) { //{n, use_sem, 0}
  pid_t pids[2 * TOTAL_PAIR_PROCESSES];
  libc_fprintf(STDERR, "\nStarting test_sync\n");
  libc_fprintf(STDERR, "Arguments are n = %s, use_sem = %s\n", argv[1], argv[2]);

  if (argc != 3){
    libc_fprintf(STDERR, "Usage: test_sync <n> <use_sem>\n");
    return -1;
  }
  char *argvDec[] = {"process_name_dec", argv[1], "-1", argv[2], NULL};
  char *argvInc[] = {"process_name_inc", argv[1], "1", argv[2], NULL};

  global = 0;

  uint64_t i;
  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    pids[i] = libc_create_process((main_function) my_process_inc, 1, argvDec, 4); // todo -> chequear
    pids[i + TOTAL_PAIR_PROCESSES] = libc_create_process((main_function) my_process_inc, 1, argvInc, 4);
  }

  // libc_fprintf(STDERR, "Final value: %d\n", global);
  
  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    if((pids[i] == 5) || (pids[i] == 7) || (pids[i] == 8) || (pids[i + TOTAL_PAIR_PROCESSES] == 5) || (pids[i + TOTAL_PAIR_PROCESSES] == 7) || (pids[i + TOTAL_PAIR_PROCESSES]==8)){
      libc_printf(".");
    }
    libc_fprintf(STDOUT, "Waiting for pids %d and %d\n", pids[i], pids[i + TOTAL_PAIR_PROCESSES]);
    if(libc_wait(pids[i], NULL) == -1){
      libc_fprintf(STDOUT, "test_sync: ERROR waiting for process %d\n", pids[i]);
    }else{
      libc_printf("Good wait %d (1) \n", pids[i]);
    }
    // libc_fprintf(STDOUT, "Waiting for %d\n", pids[i + TOTAL_PAIR_PROCESSES]);
    if(libc_wait(pids[i + TOTAL_PAIR_PROCESSES], NULL) == -1){
      libc_fprintf(STDOUT, "test_sync: ERROR waiting for process (2) %d\n", pids[i + TOTAL_PAIR_PROCESSES]);
    }else{
      libc_printf("Good wait %d (2) \n", pids[i+ TOTAL_PAIR_PROCESSES]);
    }
  }
  

  // printf("Final value: %d\n", global);
  libc_fprintf(STDERR, "Final value: %d\n", global);

  // libc_sem_close(SEM_ID); // todo -> sacar esto !!!

  libc_kill(libc_get_pid());
  return 0;
}

