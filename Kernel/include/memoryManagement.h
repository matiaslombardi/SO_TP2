#ifndef MEMORYMANAGEMENT_H
#define MEMORYMANAGEMENT_H

#include <stdint.h>
#include <library.h>
#include <memDump.h>

#define HEAPSIZE (1024 * 1024 * 128) /*128MB, debe ser multiplo de 8*/
#define NULL ((void *)0)

void mmInit(void *initAddress);

void *mmMalloc(uint64_t size);

void mmFree(void *ptr);

void fillMemInfo(char *buffer);

#endif