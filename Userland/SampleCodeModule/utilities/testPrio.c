// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include <standardIO.h>
#include <syscalls.h>
#include <string.h>
#include <tests.h>

#define MINOR_WAIT 1000000                               // TODO: To prevent a process from flooding the screen
#define WAIT      10000000                              // TODO: Long enough to see theese processes beeing run at least twice

uint64_t prioGetpid() {
    return getPid();
}

uint64_t prioCreateProcess(uint64_t *entryPoint) {
    return createProcess(entryPoint, 0, 0, 1, 0, 0, 0);
}

uint64_t prioNice(uint64_t pid, uint64_t newPrio) {
    changeProcessPriority(pid, newPrio);
    return 0;
}

uint64_t prioKill(uint64_t pid) {
    killProcess(pid);
    return 0;
}

uint64_t prioBlock(uint64_t pid) {
    changeProcessState(pid);
    return 0;
}

uint64_t prioUnblock(uint64_t pid) {
    changeProcessState(pid);
    return 0;
}

void prioBusyWait(uint64_t n) {
    uint64_t i;
    for (i = 0; i < n; i++);
}

void prioEndlessLoop() {
    uint64_t pid = prioGetpid();

    while (1) {
        char b[10]={0};
        itoaTruncate(pid, b, 10);
        print(b);
        prioBusyWait(MINOR_WAIT);
    }
}

#define TOTAL_PROCESSES 3

void testPrio() {
    uint64_t pids[TOTAL_PROCESSES];
    uint64_t i;

    for (i = 0; i < TOTAL_PROCESSES; i++)
        pids[i] = prioCreateProcess((uint64_t * ) & prioEndlessLoop);

    prioBusyWait(WAIT);
    println("");
    println("CHANGING PRIORITIES...");

    for (i = 0; i < TOTAL_PROCESSES; i++) {
        switch (i % 3) {
            case 0:
                prioNice(pids[i], 1); //lowest priority
                break;
            case 1:
                prioNice(pids[i], 3); //medium priority
                break;
            case 2:
                prioNice(pids[i], 5); //highest priority
                break;
        }
    }

    prioBusyWait(WAIT);
    println("");
    println("BLOCKING...");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        prioBlock(pids[i]);

    println("CHANGING PRIORITIES WHILE BLOCKED...");
    for (i = 0; i < TOTAL_PROCESSES; i++) {
        switch (i % 3) {
            case 0:
                prioNice(pids[i], 1); //medium priority
                break;
            case 1:
                prioNice(pids[i], 1); //medium priority
                break;
            case 2:
                prioNice(pids[i], 1); //medium priority
                break;
        }
    }

    println("UNBLOCKING...");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        prioUnblock(pids[i]);

    prioBusyWait(WAIT);
    println("");
    println("KILLING...");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        prioKill(pids[i]);
}
//
//int main(){
//  test_prio();
//  return 0;
//}
