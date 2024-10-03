
#ifndef PROCESS_H
#define PROCESS_H

#include <memory_management.h>
#include <stack.h>
#include <stdint.h>

#define FREE 0
#define READY 1
#define BLOCKED 2
#define READY 3

// el status puede ser: READY, BLOCKED, ZOMBIE, FREE, (¿RUNNING? -> por ahora no)

//    enum State{
//        READY,
//        BLOCKED,
//        ZOMBIE,
//        FREE
//        TERMINATED
//        // RUNNING??
//    }

// queue/lista de PIDs libres

typedef struct PCB{
    int64_t pid, ppid; // buscar de donde sacar el pid_t o usar uint
    uint64_t rsp;
    uint8_t status; // todo -> definir // podríamos llegar a sacarlo

    // priority

}PCB;



#endif
