#ifndef PCB_H
#define PCB_H

#define READY 0
#define BLOCKED 1
#define KILLED 2

typedef struct PCB {
    unsigned int pid;
    unsigned int state;
}PCB;

#endif