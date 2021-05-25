#ifndef LIB_64_H
#define LIB_64_H

#include <types.h>
#include <stdint.h>

typedef struct Params{
    uint64_t call;
    uint64_t first;
    uint64_t second;
    uint64_t third;
    uint64_t fourth;
    uint64_t fifth;
    uint64_t sixth;
    uint64_t seventh;
    uint64_t eighth;
} Params;

uint64_t _syscall(Params * p);

void invalidOpCode();

#endif