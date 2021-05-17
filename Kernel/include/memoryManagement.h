#ifndef MEMORYMANAGEMENT_H
#define MEMORYMANAGEMENT_H

/* Este include se debe cambiar por aquel que contenga la
    libreria de syscalls de nuestro tp :) */
#include <stdint.h> //En el tp usamos stdint.h
#include <library.h>
#include <memDump.h>

#define HEAPSIZE (1024 * 1024 * 128) /*8MB, debe ser multiplo de 8*/
#define NULL ((void *)0)

void printHeapDir();
void * mmMalloc(uint64_t size); //Cambiar int por nuestro equivalente a size_t
void mmFree(void * ptr);
void fillMemInfo(char * buffer);

#endif