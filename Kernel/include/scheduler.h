
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

void initScheduler();

uint64_t *  createProcess(uint64_t * entryPoint); //antes retornaba void

unsigned int switchProcesses(uint64_t * rsp);

void switchStates(unsigned int pid);

void endProcess(unsigned int processID);

void printProcesses();

#endif //SO_TP2_SCHEDULER_H
