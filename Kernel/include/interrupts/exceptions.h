

#ifndef TPE_ARQUI_EXCEPTIONS_H
#define TPE_ARQUI_EXCEPTIONS_H

#include <video.h>
#include <idt_loader.h>
#include <syscalls_kernel.h>
#include <interrupts.h>
#include <keyboard.h>

void exception_dispatcher ( uint64_t exception );

#endif
