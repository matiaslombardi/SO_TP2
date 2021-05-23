#ifndef SO_TP2_SCHEDULER_H
#define SO_TP2_SCHEDULER_H

#include <defs.h>
#include <circularQueue.h>
#include <pcb.h>
#include <memoryManagement.h>
#include <stdint.h>
#include <library.h>
#include <string.h>
#include <memDump.h>
#include <interrupts.h>
#include <pipes.h>

void initScheduler();

unsigned int createProcess(uint64_t *entryPoint, int foreground, uint64_t fdIn, uint64_t fdOut ,uint64_t first, uint64_t second,
                           uint64_t third); //antes retornaba void

uint64_t * switchProcesses(uint64_t * rsp);

void switchStates(unsigned int pid);

void sleep(unsigned int pid);

void wakeup(unsigned int pid);

void endProcess(unsigned int pid);

void printProcesses();

void changePriorities(unsigned int pid, unsigned int newPriority);

unsigned int getPid();

unsigned int getFdIn();

unsigned int getFdOut();

int addWaitingPid(unsigned int pid);

void exit();

#endif //SO_TP2_SCHEDULER_H
