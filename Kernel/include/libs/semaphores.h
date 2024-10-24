#ifndef _SEMAPHORES_H
#define _SEMAPHORES_H

#include <queueADT.h>
#include <shared_defs.h>
#include <scheduler.h>
#include <process.h>
#include <memory_management.h>

void acquire(uint8_t * lock);
void release(uint8_t * lock);
int64_t my_sem_open(int64_t sem_id, int value);
int64_t my_sem_post(int64_t sem_id);
int64_t my_sem_wait(int64_t sem_id);

#endif