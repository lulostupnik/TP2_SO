#ifndef _SEMAPHORES_H
#define _SEMAPHORES_H

#include <queueADT.h>
#include <shared_defs.h>
#include <scheduler.h>
#include <process.h>

void acquire(uint8_t * lock);
void release(uint8_t * lock);

#endif
