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

static int num_philosophers; 
static philosopher_t philos_array[MAX_PHILOS];
static int philosophers_pids[MAX_PHILOS];
static int64_t mutex;
static int64_t change_sem;


int canEat(int i) {
    int left = (i + num_philosophers - 1) % num_philosophers;
    int right = (i + 1) % num_philosophers;
    return philos_array[left].state != EATING && philos_array[right].state != EATING;
}

void displayState() {
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

void think(int i) {
    sys_nano_sleep(18*1); // 1 second
}

void eat(int i) {
    sys_nano_sleep(18*2); // 2 seconds
}

void takeForks(int i) {
    philos_array[i].state = HUNGRY;
    if (canEat(i)) {
        philos_array[i].state = EATING;
        libc_sem_wait(philos_array[i].leftFork);
        libc_sem_wait(philos_array[i].rightFork);
    }
}

void putForks(int i) {
    philos_array[i].state = THINKING; 
    libc_sem_post(philos_array[i].leftFork);
    libc_sem_post(philos_array[i].rightFork);
}

int philosopher(char ** argv, int argc) {
    if(argc != 2){

    }
    int i = satoi(argv[1]);

    if(i < 0){
        //@todo
        libc_printf("Noooooo");
    }
    libc_printf("I am a philo and my pid is %d\n", libc_get_pid());

    philosophers_pids[i] = libc_get_pid();
    while (1) {
        think(i);
        libc_sem_wait(mutex);
        takeForks(i);
        libc_sem_post(mutex);
        if (philos_array[i].state == EATING) {
            eat(i);
            libc_sem_wait(mutex);
            putForks(i);
            libc_sem_post(mutex);
        }
        displayState();
    }
    return 0;
}

void addPhilosopher() {
    libc_sem_wait(change_sem);
    if (num_philosophers < MAX_PHILOS) {
        int i = num_philosophers;
        philos_array[i].state = THINKING; 
        philos_array[i].leftFork = sys_sem_open_get_id(1);
        if(philos_array[i].leftFork == -1){
            libc_fprintf(STDERR, "Could open sem 1");
                //@todo smthing
        }
        philos_array[i].rightFork = sys_sem_open_get_id(1);
        if(philos_array[i].rightFork == -1){
            libc_fprintf(STDERR, "Could open sem 2");
                //@todo smthing
        }
        libc_printf("Opened sem %d and %d for philo %d\n", philos_array[i].leftFork, philos_array[i].rightFork, i);

        char philo_num_buff[5];
        char * philo_number_str = itoa(i, philo_num_buff,5,10);

        if(philo_number_str == NULL){
            //@todo some
        }

        char *args[] = {"filo" , philo_number_str};
        
        fd_t fds[CANT_FDS];
        if(sys_get_my_fds(fds) == -1){
            //
        }
        int64_t ans = libc_create_process(&philosopher, LOW, args, 2, fds);
        if(ans >= 0){
            num_philosophers++;
        }else{
            libc_fprintf(STDERR, "Couldnt create philosopher");
        }

    }
    libc_sem_post(change_sem);
}

void removePhilosopher() {
    libc_sem_wait(change_sem);
    num_philosophers--;
    libc_sem_close(philos_array[num_philosophers].leftFork);
    libc_sem_close(philos_array[num_philosophers].rightFork);
    if(libc_kill(philosophers_pids[num_philosophers]) == 0){
        libc_printf("I just killed pid %d\n", philosophers_pids[num_philosophers]);
    }else{
        libc_fprintf(STDERR, "MOOOO i tried to kill pid %d\n", philosophers_pids[num_philosophers]);
        while(1);
    }
 
    libc_sem_post(change_sem);
}

int handleKeyboard(char key) {
    if (key == 'a') {
        addPhilosopher();
        return CONTINUE;
    } else if (key == 'r') {
        if(num_philosophers > MIN_PHILOS)
            removePhilosopher();
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
    num_philosophers = 0;

    //libc_printf("%d %d \n\n", sys_sem_open_get_id(1), sys_sem_open_get_id(1));

    mutex = sys_sem_open_get_id(1);
    if(mutex == -1){
        libc_fprintf(STDERR, "Error opening sem\n");
        return;
    }
    change_sem = sys_sem_open_get_id(1);
    if(change_sem == -1){
        libc_sem_close(mutex);
        libc_fprintf(STDERR, "Error opening sem\n");
        return;
    }

    displayHeader();  

    for (int i = 0; i < MIN_PHILOS; i++) {
        addPhilosopher();
    }

    int flag = 1;
    char key;
    while (flag) {
        key = libc_get_char();
        flag = handleKeyboard(key);
    }

    while (num_philosophers > 0) {
        removePhilosopher();    
    }
    
    libc_sem_close(change_sem);
    libc_sem_close(mutex);

    return 0;
}
