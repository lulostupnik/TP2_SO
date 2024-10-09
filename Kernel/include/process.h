
#ifndef PROCESS_H
#define PROCESS_H

#include <memory_management.h>
#include <stack.h>
#include <stdint.h>
#include <scheduler.h>
#include <pcb.h>


#define READY 1
#define BLOCKED 2
#define FREE 0  //@todo esto deberia estar en pcb.h pero no compila. Igualmente pcb.h deberia estar aca pero tampoco compila. 
#define ZOMBIE 3

// typedef enum {
// 	READY,BLOCKED,FREE
// } status_t;


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
typedef enum { LOW, MEDIUM, HIGH } priority_t;

// typedef struct PCB{
//     int64_t pid, ppid; // buscar de donde sacar el pid_t o usar uint
//     uint64_t rsp;
//     uint8_t status; // todo -> definir // podríamos llegar a sacarlo
//     // priority
// }PCB;


int64_t new_process(uint64_t rip, priority_t priority);
PCB * get_pcb(int64_t pid);


#endif
