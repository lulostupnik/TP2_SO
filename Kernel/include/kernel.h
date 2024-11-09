#ifndef KERNEL_H
#define KERNEL_H


#include <stdint.h>
#include <string.h>
#include <module_loader.h>
#include <video.h>
#include <idtLoader.h>
#include <syscalls.h>
#include <keyboard.h>
#include <memory_management.h>
#include <shared_libc.h>
#include <process.h>
#include <scheduler.h>
#include <pipe.h>
#include <time.h>

int main();

#endif