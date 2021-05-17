#include <stdint.h>
#include <standardIO.h>
#include <syscalls.h>
#include <string.h>

#define MINOR_WAIT 1000000                               // TODO: To prevent a process from flooding the screen
#define WAIT      10000000                              // TODO: Long enough to see theese processes beeing run at least twice

uint64_t my_getpid() {
    return getPid();
}

uint64_t my_create_process(uint64_t *entryPoint) {
    return createProcess(entryPoint, 0);
}

uint64_t my_nice(uint64_t pid, uint64_t newPrio) {
    changeProcessPriority(pid, newPrio);
    return 0;
}

uint64_t my_kill(uint64_t pid) {
    killProcess(pid);
    return 0;
}

uint64_t my_block(uint64_t pid) {
    changeProcessState(pid);
    return 0;
}

uint64_t my_unblock(uint64_t pid) {
    changeProcessState(pid);
    return 0;
}

void bussy_wait(uint64_t n) {
    uint64_t i;
    for (i = 0; i < n; i++);
}

void endless_loop() {
    uint64_t pid = my_getpid();

    while (1) {
        char b[10]={0};
        itoaTruncate(pid, b, 10);
        print(b);
        bussy_wait(MINOR_WAIT);
    }
}

#define TOTAL_PROCESSES 3

void test_prio() {
    uint64_t pids[TOTAL_PROCESSES];
    uint64_t i;

    for (i = 0; i < TOTAL_PROCESSES; i++)
        pids[i] = my_create_process((uint64_t * ) & endless_loop);

    bussy_wait(WAIT);
    println("");
    println("CHANGING PRIORITIES...");

    for (i = 0; i < TOTAL_PROCESSES; i++) {
        switch (i % 3) {
            case 0:
                my_nice(pids[i], 1); //lowest priority
                break;
            case 1:
                my_nice(pids[i], 3); //medium priority
                break;
            case 2:
                my_nice(pids[i], 5); //highest priority
                break;
        }
    }

    bussy_wait(WAIT);
    println("");
    println("BLOCKING...");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        my_block(pids[i]);

    println("CHANGING PRIORITIES WHILE BLOCKED...");
    for (i = 0; i < TOTAL_PROCESSES; i++) {
        switch (i % 3) {
            case 0:
                my_nice(pids[i], 1); //medium priority
                break;
            case 1:
                my_nice(pids[i], 1); //medium priority
                break;
            case 2:
                my_nice(pids[i], 1); //medium priority
                break;
        }
    }

    println("UNBLOCKING...");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        my_unblock(pids[i]);

    bussy_wait(WAIT);
    println("");
    println("KILLING...");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        my_kill(pids[i]);
}
//
//int main(){
//  test_prio();
//  return 0;
//}
