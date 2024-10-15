#ifndef _PCB_H
#define _PCB_H


// typedef struct PCB{
 
//     //uint64_t pid, ppid; // buscar de donde sacar el pid_t o usar uint
//     uint64_t rsp;
//     uint8_t status; // todo -> definir // podríamos llegar a sacarlo
//     char ** argv;
//     uint64_t argc;

//     // priority
// }PCB;

typedef enum { LOW=0, MEDIUM, HIGH } priority_t;

typedef struct PCB{
    int64_t pid, ppid; // buscar de donde sacar el pid_t o usar uint
    uint64_t rsp;
    uint64_t base_pointer;
    //uint8_t is_foreground;
    uint8_t status; // todo -> definir // podríamos llegar a sacarlo
    char ** argv;
    uint64_t argc;
    int64_t ret;
    priority_t priority;
    uint8_t killable;
    //uint64_t base_pointer;
}PCB;


#endif