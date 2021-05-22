#ifndef SO_TP2_SEMAPHORE_H
#define SO_TP2_SEMAPHORE_H

#include <scheduler.h>
#include <string.h>
#include <library.h>
#include <queue.h>

int semOpen(char *semId, uint64_t initialValue);

int semClose(char *semId);

int semPost(char *semId);

int semWait(char * semId);


#endif //SO_TP2_SEMAPHORE_H
