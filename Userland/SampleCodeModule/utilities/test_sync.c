#include <stdint.h>
#include <standardIO.h>
#include <syscalls.h>
#include "test_util.h"
#include <lib64.h>


uint64_t my_create_process(uint64_t *entryPoint, uint64_t sem, uint64_t inc, uint64_t iter) {
    return createProcess(entryPoint, 0, 0, 1, sem, inc, iter);
}

uint64_t my_sem_open(char *sem_id, uint64_t initialValue) {
    return semOpen(sem_id, initialValue);
}

uint64_t my_sem_wait(char *sem_id) {
    return semWait(sem_id);
}

uint64_t my_sem_post(char *sem_id) {
    return semPost(sem_id);
}

uint64_t my_sem_close(char *sem_id) {
    return semClose(sem_id);
}

#define TOTAL_PAIR_PROCESSES 2
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

    if (sem && !my_sem_open(SEM_ID, 1)) {
        print("ERROR OPENING SEM\n");
        return;
    }

    for (i = 0; i < N; i++) {
        if (sem) my_sem_wait(SEM_ID);
        slowInc(&global, value);
        printc(", ", 0x00FF00);
        printInt(global);
        if (sem) my_sem_post(SEM_ID);
    }

    if (sem) my_sem_close(SEM_ID);

    print("global: ");
    printInt(global);
    println("");
    _exit(1);
}

void test_sync() {
    uint64_t i;

    global = 0;

    print("CREATING PROCESSES...(WITH SEM)\n");

    for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
        my_create_process((uint64_t * ) & inc, 1, 1, 50);
        my_create_process((uint64_t * ) & inc, 1, 1, 50);
    }
}

void test_no_sync() {
    uint64_t i;

    global = 0;

    print("CREATING PROCESSES...(WITHOUT SEM)\n");

    for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
        my_create_process((uint64_t * ) & inc, 0, 1, 500);
        my_create_process((uint64_t * ) & inc, 0, 1, 500);
    }
}
//
//int main(){
//  test_sync();
//  return 0;
//}
