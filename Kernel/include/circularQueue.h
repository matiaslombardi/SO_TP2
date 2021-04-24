//
// Created by pau on 24/4/21.
//

#ifndef CIRCULARQUEUE_H
#define CIRCULARQUEUE_H

#include <pcb.h>
#include <memoryManagement.h>

typedef struct Queue * QueueADT;

QueueADT newQueue();

void freeQueue(QueueADT queue);

void push(QueueADT queue, PCB pcb);

PCB pop(QueueADT queue);

int isEmpty(QueueADT queue);

//void printQueue(QueueADT queue);

#endif
