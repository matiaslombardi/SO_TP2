#ifndef CIRCULARQUEUE_H
#define CIRCULARQUEUE_H

#include <pcb.h>
#include <memoryManagement.h>
#include <defs.h>
#include <library.h>
#include <memDump.h>

typedef struct Queue * QueueADT;

QueueADT newQueue();

void freeQueue(QueueADT queue);

void push(QueueADT queue, PCB * pcb);

PCB * pop(QueueADT queue);

int isEmpty(QueueADT queue);

PCB * findPCB(QueueADT queue, unsigned int pid);

PCB * deleteNode(QueueADT queue, unsigned int pid);

void toBegin(QueueADT queue);

int hasNext(QueueADT queue);

PCB * next(QueueADT queue);

void printQueue(QueueADT queue);

#endif
