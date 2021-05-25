// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include <standardIO.h>
#include <syscalls.h>
#include <testUtil.h>
#include <lib64.h>
#include <tests.h>


uint64_t syncCreateProcess(uint64_t *entryPoint, uint64_t sem, uint64_t inc, uint64_t iter) {
    return createProcess(entryPoint, 0, 0, 1, sem, inc, iter, "iterator");
}

uint64_t syncSemOpen(char *sem_id, uint64_t initialValue) {
    return semOpen(sem_id, initialValue);
}

uint64_t syncSemWait(char *sem_id) {
    return semWait(sem_id);
}

uint64_t syncSemPost(char *sem_id) {
    return semPost(sem_id);
}

uint64_t syncSemClose(char *sem_id) {
    return semClose(sem_id);
}

#define TOTAL_PAIR_PROCESSES 3
#define SEM_ID "sem"

int64_t global;  //shared memory

void slowInc(int64_t *p, int64_t inc) {
    int64_t aux = *p;
    aux += inc;
    yield();
    *p = aux;
}

void inc(uint64_t sem, int64_t value, uint64_t N) {

    uint64_t i;

    if (sem && !syncSemOpen(SEM_ID, 1)) {
        print("ERROR OPENING SEM\n");
        return;
    }

    for (i = 0; i < N; i++) {
        if (sem) syncSemWait(SEM_ID);
        slowInc(&global, value);
        printc(", ", 0x00FF00);
        printInt(global);
        if (sem) syncSemPost(SEM_ID);
    }

    if (sem) syncSemClose(SEM_ID);

    print("global: ");
    printInt(global);
    println("");
    _exit(1);
}

void testSync() {
    uint64_t i;

    global = 0;

    print("CREATING PROCESSES...(WITH SEM)\n");

    for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
        syncCreateProcess((uint64_t * ) & inc, 1, 1, 500);
        syncCreateProcess((uint64_t * ) & inc, 1, 1, 500);
    }
}

void testNoSync() {
    uint64_t i;

    global = 0;

    print("CREATING PROCESSES...(WITHOUT SEM)\n");

    for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
        syncCreateProcess((uint64_t * ) & inc, 0, 1, 500);
        syncCreateProcess((uint64_t * ) & inc, 0, 1, 500);
    }
}
//
//int main(){
//  test_sync();
//  return 0;
//}
