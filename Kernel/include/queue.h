#ifndef SO_TP2_QUEUE_H
#define SO_TP2_QUEUE_H

#include <memoryManagement.h>
#include <defs.h>
#include <library.h>
#include <memDump.h>

typedef struct Queue * QueueADT;

QueueADT newQueue();

void freeQueue(QueueADT queue);

void enqueue(QueueADT queue, int pid);

int dequeue(QueueADT queue);

int isEmpty(QueueADT queue);

void deleteWaiting(QueueADT queue, int pid);

void toBegin(QueueADT queue);

int hasNext(QueueADT queue);

int next(QueueADT queue);

void printQueue(QueueADT queue);

#endif //SO_TP2_QUEUE_H
