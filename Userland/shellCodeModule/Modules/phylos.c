// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com


#include <phylos.h>
#include <stdio.h>
#include <stddef.h>  

char* itoa(int value, char* str, size_t size, int base) {
    if (str == NULL || size == 0) {
        return NULL;
    }
    
    if (base < 2 || base > 36) {
        *str = '\0';  
        return NULL;
    }

    char buffer[66];  
    char* ptr = buffer + sizeof(buffer) - 1;
    *ptr = '\0';

    int is_negative = 0;

   
    if (value == 0) {
        if (size < 2) {
            *str = '\0'; 
            return NULL;
        }
        str[0] = '0';
        str[1] = '\0';
        return str;
    }

    uint64_t uvalue;
    if (value < 0 && base == 10) {
        is_negative = 1;
        uvalue = (uint64_t)(-value);
    } else {
        uvalue = (uint64_t)value;
    }

    while (uvalue != 0) {
        int remainder = uvalue % base;
        *(--ptr) = (remainder > 9) ? (remainder - 10) + 'a' : remainder + '0';
        uvalue /= base;
    }

    if (is_negative) {
        *(--ptr) = '-';
    }

   size_t len = (buffer + sizeof(buffer) - 1) - ptr;

    if (len + 1 > size) {
        *str = '\0';
        return NULL;
    }

    for (size_t i = 0; i < len + 1; ++i) {  
        str[i] = ptr[i];
    }

    return str;
}


typedef struct {
    int state;
    int64_t leftFork;
    int64_t rightFork;
} philosopher_t;

static int64_t forks[MAX_PHILOS];
static philosopher_t philos_array[MAX_PHILOS];
static int philosophers_pids[MAX_PHILOS];
static int64_t state_mutex;
static int64_t added_min_sem; 
static int64_t mutex_add_delete; 
static int64_t num_philos_mutex;



// . E . E 
// Mato siempre al primero que esta comiendo
// Agrego al final 
void think(int i) {
    sys_nano_sleep(3); // 1 second
}

void eat(int i) {
    libc_sem_wait(state_mutex);
    philos_array[i].state = EATING; 
    libc_sem_post(state_mutex);
    
    sys_nano_sleep(10); 

    libc_sem_wait(state_mutex);
    philos_array[i].state = THINKING; 
    libc_sem_post(state_mutex);
}

int philosopher(char ** argv, int argc) {
    if(argc != 2){

    }
    int i = satoi(argv[1]);

    if(i < 0){
        //@todo
      
    }
    if(i < MIN_PHILOS){
        libc_sem_wait(added_min_sem);
    }
    
    while (1) {
        think(i);
      

        libc_sem_wait(mutex_add_delete);
        libc_sem_post(mutex_add_delete);


        if(i % 2 == 0){
            libc_sem_wait(philos_array[i].leftFork);
            libc_sem_wait(philos_array[i].rightFork);
        }else{
            libc_sem_wait(philos_array[i].rightFork);
            libc_sem_wait(philos_array[i].leftFork);
        }
    
        eat(i);
        libc_sem_post(philos_array[i].leftFork);
        libc_sem_post(philos_array[i].rightFork);

    }
    return 0;
}


static int64_t create_process(int64_t i){
    char philo_num_buff[5];
    char * philo_number_str = itoa(i, philo_num_buff,5,10);
    int64_t ans;
    if(philo_number_str == NULL){
            //@todo some
    }
    char *args[] = {"filo" , philo_number_str};
    fd_t fds[CANT_FDS];
  
    if(sys_get_my_fds(fds) == -1){
            //
    }
    return libc_create_process(&philosopher, LOW, args, 2, fds);
}

int64_t addFisrtPhylos(){
    int64_t num_philosophers = 0;
    philos_array[0].leftFork=  forks[0] = sys_sem_open_get_id(1);
    philos_array[0].rightFork = forks[1] = sys_sem_open_get_id(1);
    philos_array[1].rightFork =  philos_array[0].leftFork;
    philos_array[1].leftFork =  philos_array[0].rightFork;

    for(int i=2; i<MIN_PHILOS ; i++){
        philos_array[i].leftFork = forks[i] = sys_sem_open_get_id(1);
        philos_array[i-1].rightFork =  philos_array[i].leftFork;
        philos_array[i].rightFork = philos_array[0].leftFork;
    }

    for(int i=0; i<MIN_PHILOS ; i++){
        philosophers_pids[i] = create_process(i);
        if(philosophers_pids[i] >= 0){
            num_philosophers++;
        }
    }
    for(int i=0; i<MIN_PHILOS ; i++){
        libc_sem_post(added_min_sem);
    }
    return num_philosophers;
}


static void addPhilosopher(int64_t * num_philosophers) {
    if ((*num_philosophers) < MAX_PHILOS) {
        int i = (*num_philosophers);
        philos_array[i].state = THINKING; 

        int64_t new_sem;
        if((new_sem = sys_sem_open_get_id(1)) < 0){
            libc_fprintf(STDERR, "Couldnt open sem 1");
                //
        }
        forks[i] = new_sem;
        libc_sem_wait(mutex_add_delete);

        philos_array[i-1].rightFork = new_sem;
        philos_array[i].leftFork = new_sem;
        philos_array[i].rightFork = philos_array[0].leftFork;
        philosophers_pids[i] = create_process(i);
        if(philosophers_pids[i] >= 0){
            (*num_philosophers)++;
        }else{
            libc_sem_close(forks[i]);
            libc_fprintf(STDERR, "Couldnt create philosopher");
        }
        libc_sem_post(mutex_add_delete);
        
        }

        // for(int i=0; i<MAX_PHILOS; i++){
        //     libc_printf("i %d, state %d, left %d, right %d\n", i, philos_array[i].state, philos_array[i].leftFork, philos_array[i].rightFork);
        // }
}


void displayState(int64_t num_philosophers) {
    libc_printf("");
    libc_sem_wait(state_mutex);
    for (int i = 0; i < num_philosophers; i++) {
        if (philos_array[i].state == EATING) { 
            libc_put_char('E');
        } else {
            libc_put_char('.');
        }
        libc_put_char(' ');
    }
    libc_printf("\n");
  //  libc_ps();
    libc_sem_post(state_mutex);
         
}

int handleKeyboard(char key, int64_t * num_phil) {
    if (key == 'a') {
        addPhilosopher(num_phil);
        return CONTINUE;
    } else if (key == 'r') {
        if(*num_phil > MIN_PHILOS)
            //removePhilosopher();
        return CONTINUE;
    } else if (key == 'e') {
        return FINISH;
    }
    return CONTINUE;
}



void displayHeader() {
    libc_printf("\n\tInstructions:\n");
    libc_printf("\tPress - a - Add philosopher\n");
    libc_printf("\tPress - r - Remove philosopher\n");
    libc_printf("\tPress - e - Exit\n");
    libc_printf("\tState of philos_array:\n");
    libc_printf(" - E - Comiendo\n");
    libc_printf(" - . - Pensando\n\n\n");
}


int phylos(char ** argv, int argc) {
    int64_t num_philosophers = 0;

    state_mutex = sys_sem_open_get_id(1);
    if(state_mutex == -1){
        libc_fprintf(STDERR, "Error opening sem\n");
        return;
    }
    added_min_sem = sys_sem_open_get_id(0);
    mutex_add_delete = sys_sem_open_get_id(1);
    num_philos_mutex = sys_sem_open_get_id(1);
    displayHeader(); 
    num_philosophers = addFisrtPhylos();

    int flag = 1;
    char key;
    while (flag) {
        displayState(num_philosophers);
        key = libc_get_char();
        flag = handleKeyboard(key, &num_philosophers);
    }

    while (num_philosophers > 0) {
        //removePhilosopher();    
    }
    
    // libc_sem_close(change_sem);
    libc_sem_close(state_mutex);

    return 0;
}





