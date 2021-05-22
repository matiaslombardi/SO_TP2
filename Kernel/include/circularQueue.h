#ifndef CIRCULARQUEUE_H
#define CIRCULARQUEUE_H

#include <pcb.h>
#include <memoryManagement.h>
#include <defs.h>
#include <library.h>
#include <memDump.h>

typedef struct CircularQueue * CircularQueueADT;

CircularQueueADT newCircularQueue();

void freeCircularQueue(CircularQueueADT queue);

void circularEnqueue(CircularQueueADT queue, PCB * pcb);

PCB * circularDequeue(CircularQueueADT queue);

int isEmptyCircular(CircularQueueADT queue);

PCB * findPCB(CircularQueueADT queue, unsigned int pid);

PCB * deleteNode(CircularQueueADT queue, unsigned int pid);

void circularToBegin(CircularQueueADT queue);

int circularHasNext(CircularQueueADT queue);

PCB * circularNext(CircularQueueADT queue);

void printCircularQueue(CircularQueueADT queue);

#endif
