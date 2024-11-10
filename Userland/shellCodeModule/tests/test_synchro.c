// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <test_synchro.h>

int64_t global; 

void slowInc(int64_t *p, int64_t inc) {
  uint64_t aux = *p;
  libc_yield(); 
  aux += inc;
  *p = aux;
}

int64_t check_params(char *argv[], uint64_t argc, int64_t * n, int8_t * inc, int8_t * use_sem){
  
  if (argv == NULL || argc != 4){
    return -1;
  }

  int64_t satoi_flag;
  int8_t use_sem_aux, inc_aux;
  int64_t n_aux;
  if ((n_aux = libc_satoi(argv[1], &satoi_flag)) <= 0 || !satoi_flag)
    return -1;
  if ((inc_aux = libc_satoi(argv[2], &satoi_flag)) == 0 || !satoi_flag)
    return -1;
  if ((use_sem_aux = libc_satoi(argv[3], &satoi_flag)) < 0 || !satoi_flag)
    return -1;

   if(n != NULL){
      *n = n_aux;
   }
   if(inc != NULL){
      *inc = inc_aux;
   }
   if(use_sem != NULL){
      *use_sem = use_sem_aux;
   }

  return 0;
}

uint64_t my_process_inc(char *argv[], uint64_t argc) {
  int64_t n;
  int8_t inc;
  int8_t use_sem;

  if(check_params(argv, 4, &n, &inc, &use_sem) != 0 ){
    libc_fprintf(STDERR, "Error: Usage: test_sync <n> <inc> <use_sem>");  
    return -1;
  }
 

  if (use_sem){
    if (libc_sem_open(SEM_ID, 1) == -1) {
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
      if(libc_sem_post(SEM_ID) == -1){
        libc_fprintf(STDERR, "test_sync: ERROR posting semaphore\n");
        return -1;
      }
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
  
  char *argvDec[] = {"test_sync_dec", argv[1], "-1", argv[2], NULL};
  char *argvInc[] = {"test_sync_inc", argv[1], "1", argv[2], NULL};

  if(check_params(argvDec, 4, NULL, NULL, NULL) != 0 || check_params(argvInc, 4, NULL, NULL, NULL)){
    libc_fprintf(STDERR, "Usage: test_sync <n> <use_sem>\n");
    return -1;
  }

  global = 0;

  uint64_t i;
  uint64_t flag = 1;
  uint64_t last_wait_idx = 0;
  for (i = 0; i < TOTAL_PAIR_PROCESSES && flag; i++, last_wait_idx++) {
    fd_t fds[] = {-1, STDERR, -1};
    pids[i] = libc_create_process((main_function) my_process_inc, 1, argvDec, 4, fds);
    pids[i + TOTAL_PAIR_PROCESSES] = libc_create_process((main_function) my_process_inc, 1, argvInc, 4, fds);
   
    if(pids[i] == -1 || pids[i+TOTAL_PAIR_PROCESSES] == -1){
      libc_fprintf(STDOUT, "test_sync: ERROR creating process %d\n", pids[i]);
      flag = 0;
    }
  }
  
  
  for (i = 0; i < last_wait_idx; i++) {
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



