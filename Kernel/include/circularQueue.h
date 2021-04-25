//
// Created by pau on 24/4/21.
//

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

PCB * findNode(QueueADT queue, unsigned int pid);

void printQueue(QueueADT queue);

#endif
