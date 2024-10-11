#ifndef _PCB_H
#define _PCB_H


// typedef struct my_PCB{
 
//     //uint64_t pid, ppid; // buscar de donde sacar el pid_t o usar uint
//     uint64_t rsp;
//     uint8_t status; // todo -> definir // podríamos llegar a sacarlo
//     char ** args;
//     uint64_t cant;

//     // priority
// }my_PCB;




typedef struct my_PCB{
 
    uint64_t pid, ppid; // buscar de donde sacar el pid_t o usar uint
    uint64_t rsp;
    uint8_t status; // todo -> definir // podríamos llegar a sacarlo
    char ** args;
    uint64_t cant;

    // priority
}my_PCB;


#endif