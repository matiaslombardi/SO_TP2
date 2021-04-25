//
// Created by matias on 24/4/21.
//

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
uint64_t * switchProcesses(uint64_t * rsp);
void switchStates(unsigned int pid);


#endif //SO_TP2_SCHEDULER_H
