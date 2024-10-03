

#ifndef TPE_ARQUI_EXCEPTIONS_H
#define TPE_ARQUI_EXCEPTIONS_H

#include <video.h>
#include <idtLoader.h>
#include <syscalls.h>
#include <interrupts.h>

void exception_dispatcher ( uint64_t exception );

#endif //TPE_ARQUI_EXCEPTIONS_H
