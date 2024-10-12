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




typedef struct PCB{
 
    uint64_t pid, ppid; // buscar de donde sacar el pid_t o usar uint
    uint64_t rsp;
    uint8_t status; // todo -> definir // podríamos llegar a sacarlo
    char ** argv;
    uint64_t argc;
    int64_t ret;
    //uint8_t priority;
}PCB;


#endif