#ifndef LIB_64_H
#define LIB_64_H

#include <types.h>
#include <stdint.h>

uint64_t _syscall(uint64_t syscall, ...);

void invalidOpCode();

void yield();

#endif