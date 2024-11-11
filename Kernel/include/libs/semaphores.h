#ifndef _SEMAPHORES_H
#define _SEMAPHORES_H

#define SEM_AMOUNT_USER 100
#define KERNEL_SEM_ID_LIMIT 350 //Kernel sems goes from ID SEM_AMOUNT_USER to KERNEL_SEM_AMOUT-1

#include <queue_adt.h>
#include <shared_defs.h>
#include <scheduler.h>
#include <process.h>
#include <memory_management.h>

void acquire ( uint8_t * lock );
void release ( uint8_t * lock );
int64_t my_sem_open ( int64_t sem_id, int value, uint8_t is_kernel );
int64_t my_sem_post ( int64_t sem_id, uint8_t is_kernel );
int64_t my_sem_wait ( int64_t sem_id, uint8_t is_kernel );
int64_t my_sem_close ( int64_t sem_id, uint8_t is_kernel );
int64_t sem_post_if_value_is_zero ( int64_t sem_id, uint8_t is_kernel );
int64_t my_sem_open_get_id ( int value );
int64_t my_sem_post_no_yield ( int sem_id );
int64_t sem_delete_from_blocked_queue ( PCB * pcb );
#endif
