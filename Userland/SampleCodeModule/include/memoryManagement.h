#ifndef SO_TP2_MEMORYMANAGEMENT_H
#define SO_TP2_MEMORYMANAGEMENT_H

#include <syscalls.h>
#include <stdint.h>

void * mmMalloc(uint64_t size);

void mmFree(void * ptr);

void getMemInfo(char * buffer);

void test_mm();

#endif //SO_TP2_MEMORYMANAGEMENT_H
