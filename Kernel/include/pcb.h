#ifndef PCB_H
#define PCB_H

#include <stdint.h>
#include <queue.h>

#define READY 0
#define BLOCKED 1
#define KILLED 2
#define RESIGN 3

#define TOTAL_MALLOCS 5

typedef struct PCB {
    unsigned int pid;
    unsigned int state;
    uint64_t *rsp;
    uint64_t *rbp;
    unsigned int priority;
    unsigned int tickets;
    unsigned int foreground;
    unsigned int fdIn;
    unsigned int fdOut;
    unsigned int waitingPid;
    char name[20];
    uint64_t mallocDirections[TOTAL_MALLOCS];
} PCB;

#endif