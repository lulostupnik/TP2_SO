

#ifndef TPE_ARQUI_EXCEPTIONS_H
#define TPE_ARQUI_EXCEPTIONS_H

#include <video.h>
#include <idtLoader.h>
#include <syscallsKernel.h>
#include <interrupts.h>
#include <keyboard.h>

void exception_dispatcher ( uint64_t exception );

#endif
