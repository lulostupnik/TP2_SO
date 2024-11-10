// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <phylos.h>
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
    int64_t left_fork;
    int64_t right_fork;
} philosopher_t;

static void think(int i);
static void eat(int i);
static int64_t philosopher(char ** argv, uint64_t argc);
static int64_t create_process(int64_t i);
static int64_t add_first_phylos();

static void add_philosopher();
static void remove_philosopher();
static void display_state();
static int8_t handle_keyboard(char key);
static void display_header();
static void clean_all();
static int64_t open_mutexes();
static void close_mutexes();

static int64_t forks[MAX_PHILOS];
static philosopher_t philos_array[MAX_PHILOS];
static int64_t philosophers_pids[MAX_PHILOS];
static int64_t state_mutex;
static int64_t added_min_sem; 
static int64_t num_philos_mutex;
static int64_t last_mutex;
static int64_t second_last_mutex;
static int64_t num_philosophers = 0;




static void think(int i) {
    libc_ticks_sleep(THINK_CONSTANT* get_uniform(GET_UNIFORM_CONSTANT));
}

static void eat(int i) {
    libc_sem_wait(state_mutex);
    philos_array[i].state = EATING;
    display_state();
    libc_sem_post(state_mutex);
    
    libc_ticks_sleep(SLEEP_CONSTANT * get_uniform(GET_UNIFORM_CONSTANT));

    libc_sem_wait(state_mutex);
    philos_array[i].state = THINKING;
    display_state(); 
    libc_sem_post(state_mutex);
}

static int64_t philosopher(char ** argv, uint64_t argc) {
    int64_t flag = 0;
    int i = libc_satoi(argv[1], &flag);
    if (flag == 0) {
        libc_fprintf(STDERR, "Error: Invalid philosopher number\n");
        return -1;
    }
    
    if(i < MIN_PHILOS){
        libc_sem_wait(added_min_sem);
    }
    
    while (1) {
        think(i);

        
        uint8_t have_last_sem = 0;
        uint8_t have_second_last_sem = 0;

        libc_sem_wait(num_philos_mutex);
    
        if(i == num_philosophers - 1){
            libc_sem_wait(last_mutex);
            have_last_sem = 1;
        } 
        else if(i == num_philosophers - 2){
            libc_sem_wait(second_last_mutex);
            have_second_last_sem = 1;
        }
        libc_sem_post(num_philos_mutex);
        


        if(i % 2 == 0){
            libc_sem_wait(philos_array[i].left_fork);
            libc_sem_wait(philos_array[i].right_fork);
        }else{
            libc_sem_wait(philos_array[i].right_fork);
            libc_sem_wait(philos_array[i].left_fork);
        }

        
    
        eat(i);

        libc_sem_post(philos_array[i].right_fork);
        libc_sem_post(philos_array[i].left_fork);
    
    
        if(have_last_sem){
            libc_sem_post(last_mutex);
        }
        else if(have_second_last_sem){
            libc_sem_post(second_last_mutex);
        }
        
        

    }
    return 0;
}


static int64_t create_process(int64_t i){
    char philo_num_buff[5];
    char * philo_number_str = itoa(i, philo_num_buff,5,10);
    if(philo_number_str == NULL){
        return -1;
    }
    char *args[] = {"filo" , philo_number_str};
    fd_t fds[CANT_FDS];
  
    if(libc_get_my_fds(fds) == -1){
        libc_fprintf(STDERR, "Error getting fds\n");
        return -1;
    }
    return libc_create_process((main_function )&philosopher, LOW, args, 2, fds);
}


static int64_t add_first_phylos(){

    for(int i = 0; i < MIN_PHILOS; i++){
        philos_array[i].left_fork = forks[i] = libc_sem_open_get_id(1);
        if(forks[i] < 0){
            for(int j = 0; j < i; j++){
                libc_sem_close(forks[j]);
            }
            return -1;
        }
    }
    for(int i = 0; i < MIN_PHILOS - 1; i++){
        philos_array[i].right_fork = forks[i + 1];
    }
    philos_array[MIN_PHILOS - 1].right_fork = forks[0];
    
    for(int i = 0; i < MIN_PHILOS ; i++){
        philosophers_pids[i] = create_process(i);
        if(philosophers_pids[i] < 0){
            for(int j = 0; j < MIN_PHILOS; j++){
                libc_sem_close(forks[j]);
            }
            for(int j = 0; j < i; j++){
                libc_kill(philosophers_pids[j]);
            }
            return -1;
        }
        
    }
    num_philosophers = MIN_PHILOS;

    for(int i = 0; i < MIN_PHILOS ; i++){
        libc_sem_post(added_min_sem);
    }
    return num_philosophers;
}


static void add_philosopher() {
    if(num_philosophers >= MAX_PHILOS){
        libc_fprintf(STDERR, "Error: Max philosophers reached\n");
        return;
    }


    int64_t new_sem;
    if((new_sem = libc_sem_open_get_id(1)) < 0){
        libc_fprintf(STDERR, "Error: Couldn't open sem to add philosopher\n");
        return;
    }
    
    libc_sem_wait(num_philos_mutex);
    int i = num_philosophers;
    philos_array[i].state = THINKING;
    forks[i] = new_sem;
    libc_sem_wait(last_mutex);


    philos_array[i - 1].right_fork = new_sem;
    philos_array[i].left_fork = new_sem;
    philos_array[i].right_fork = philos_array[0].left_fork;
    philosophers_pids[i] = create_process(i);
    if(philosophers_pids[i] >= 0){
        num_philosophers++;
    }else{
        libc_sem_close(new_sem);
        philos_array[i - 1].right_fork = philos_array[0].left_fork;
        libc_fprintf(STDERR, "Couldn't create philosopher");
    }

    libc_sem_post(last_mutex);
    libc_sem_post(num_philos_mutex);    
        
}

static void remove_philosopher() {
    if(num_philosophers == MIN_PHILOS){
        libc_fprintf(STDERR, "Error: Minimum philosophers reached\n");
        return;
    }
    
    libc_sem_wait(num_philos_mutex);
    int i = num_philosophers;
    libc_sem_wait(last_mutex);
    libc_sem_wait(second_last_mutex);

    philos_array[i - 2].right_fork = philos_array[0].left_fork;
    libc_sem_close(philos_array[i - 1].left_fork);
    libc_kill(philosophers_pids[i - 1]);
    num_philosophers--;

    libc_sem_post(second_last_mutex);
    libc_sem_post(last_mutex);
    libc_sem_post(num_philos_mutex);
    
    
        
}



static void display_state() {
    libc_printf("");
    for (int i = 0; i < num_philosophers; i++) {
        if (philos_array[i].state == EATING) { 
            libc_put_char('E');
        } else {
            libc_put_char('.');
        }
        libc_put_char(' ');
    }
    libc_printf("\n");
         
}

static int8_t handle_keyboard(char key) {
    if (key == 'a') {
        add_philosopher();
    } else if (key == 'r') {
        remove_philosopher();
    } else if (key == 'e') {
        return FINISH;
    }
    return CONTINUE;
}



static void display_header() {
    libc_printf("\n\tInstructions:\n");
    libc_printf("\tPress - a - Add philosopher\n");
    libc_printf("\tPress - r - Remove philosopher\n");
    libc_printf("\tPress - e - Exit\n");
    libc_printf("\tState of philos_array:\n");
    libc_printf(" - E - Comiendo\n");
    libc_printf(" - . - Pensando\n\n\n");
}

static void close_mutexes(){
    libc_sem_close(num_philos_mutex);
    libc_sem_close(added_min_sem);
    libc_sem_close(last_mutex);
    libc_sem_close(second_last_mutex);
    libc_sem_close(state_mutex);
    libc_sem_close(added_min_sem);
}

static void clean_all(){
    
    libc_sem_wait(num_philos_mutex);
    for(int i = 0; i < num_philosophers; i++){
        libc_kill(philosophers_pids[i]);
    }
    for(int i = 0; i < num_philosophers; i++){
        libc_sem_close(forks[i]);
    }
    close_mutexes();
    
   return;
}

static int64_t open_mutexes(){
    state_mutex = libc_sem_open_get_id(1);
    if(state_mutex == -1){
        return -1;
    }
    added_min_sem = libc_sem_open_get_id(0);
    if(added_min_sem == -1){
        libc_sem_close(state_mutex);
        return -1;
    }
    num_philos_mutex = libc_sem_open_get_id(1);
    if(num_philos_mutex == -1){
        libc_sem_close(state_mutex);
        libc_sem_close(added_min_sem);
        return -1;
    }
    last_mutex = libc_sem_open_get_id(1);
    if(last_mutex == -1){
        libc_sem_close(state_mutex);
        libc_sem_close(added_min_sem);
        libc_sem_close(num_philos_mutex);
        return -1;
    }
    second_last_mutex = libc_sem_open_get_id(1);
    if(second_last_mutex == -1){
        libc_sem_close(state_mutex);
        libc_sem_close(added_min_sem);
        libc_sem_close(num_philos_mutex);
        libc_sem_close(last_mutex);
        return -1;
    }
    return 0;
}

int64_t phylo(char ** argv, int argc) {

    if(open_mutexes() == -1){
        libc_fprintf(STDERR, "Error opening sem\n");
        return -1;
    }

    display_header(); 
    if(add_first_phylos() == -1){
        libc_fprintf(STDERR, "Error adding first philosophers\n");
        close_mutexes();
        return -1;
    }

    int8_t flag = 1;
    char key;
    while (flag) {
        key = libc_get_char();
        flag = handle_keyboard(key);
    }

    clean_all();
    
    return 0;
}









