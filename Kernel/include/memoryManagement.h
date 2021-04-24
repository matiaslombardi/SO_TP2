#ifndef MEMORYMANAGEMENT_H
#define MEMORYMANAGEMENT_H

/* Este include se debe cambiar por aquel que contenga la
    libreria de syscalls de nuestro tp :) */
#include <stdint.h> //En el tp usamos stdint.h

#define HEAPSIZE 0x800 /*8MB, debe ser multiplo de 8*/
#define NULL (void *)0

void * mmMalloc(uint64_t size); //Cambiar int por nuestro equivalente a size_t
void mmFree(void * ptr);

#endif