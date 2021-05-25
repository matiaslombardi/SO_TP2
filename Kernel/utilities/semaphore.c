// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <semaphore.h>

#define TOTAL_SEMS 25
#define SEM_ID_SIZE 30

typedef struct sem {
    char semId[SEM_ID_SIZE];
    uint64_t value;
    QueueADT blockedProcesses;
    uint64_t attached;
} sem;

static sem semaphores[TOTAL_SEMS];

static int searchSem(char *semId);

static void acquire(int *lock);

static void release(int *lock);

int lock = 0;

int semOpen(char *semId, uint64_t initialValue) {
    int len = strlen(semId);
    if (len < 0 || len > SEM_ID_SIZE - 1) {
        return 0;
    }

    acquire(&lock);

    int firstFree = -1;
    int i;
    for (i = 0; i < TOTAL_SEMS; i++) {
        if (strlen(semaphores[i].semId) != 0) {
            if (strcmp(semaphores[i].semId, semId) == 0) {
                semaphores[i].attached++;
                release(&lock);
                return 1;
            }
        } else if (firstFree == -1) {
            firstFree = i;
        }
    }

    if (firstFree != -1) {
        strcpy(semaphores[firstFree].semId, semId);
        semaphores[firstFree].blockedProcesses = newQueue();
        semaphores[firstFree].value = initialValue;
        semaphores[firstFree].attached = 1;
        release(&lock);
        return 1;
    }

    release(&lock);

    return 0;
}

int semClose(char *semId) {
    acquire(&lock);
    for (int i = 0; i < TOTAL_SEMS; i++) {
        if (strcmp(semaphores[i].semId, semId) == 0) {
            if (semaphores[i].attached == 1) {
                freeQueue(semaphores[i].blockedProcesses);
                semaphores[i].semId[0] = 0; //'\0'
            } else {
                semaphores[i].attached--;
            }

            release(&lock);
            return 1;
        }
    }
    release(&lock);
    return 0;
}

int semPost(char *semId) {
    acquire(&lock);

    int found = searchSem(semId);
    if (found != -1) {
        semaphores[found].value++;
        if (!isEmpty(semaphores[found].blockedProcesses)) {
            semaphores[found].value--;
            int pid = dequeue(semaphores[found].blockedProcesses);
            release(&lock);
            wakeup(pid);
        } else {
            release(&lock);
        }
        return 1;
    }

    release(&lock);
    return 0;
}

int semWait(char *semId) {
    acquire(&lock);
    int found = searchSem(semId);

    if (found != -1) {
        if (semaphores[found].value > 0) {
            semaphores[found].value--;
            release(&lock);
        } else {
            uint64_t pid = getPid();
            enqueue(semaphores[found].blockedProcesses, pid);
            release(&lock);
            sleep(pid);
        }
        return 1;
    }
    release(&lock);
    return 0;
}

static int searchSem(char *semId) {
    for (int i = 0; i < TOTAL_SEMS; i++) {
        if (strlen(semaphores[i].semId) > 0 && strcmp(semaphores[i].semId, semId) == 0) {
            return i;
        }
    }
    return -1;
}

void removeWaitingPid(unsigned int pid) {
    for (int i = 0; i < TOTAL_SEMS; i++) {
        if (strlen(semaphores[i].semId) > 0) {
            deleteWaiting(semaphores[i].blockedProcesses, pid);
        }
    }
}

static void acquire(int *lock) {
    while (_xchg(lock, 1) != 0);
}

static void release(int *lock) {
    _xchg(lock, 0);
}

int fillSemInfo(char *buffer) {
    char aux[64] = {0};

    strcat(buffer, "Name                          ");
    strcat(buffer, "Value    ");
    strcat(buffer, "Blocked Processes\n");

    for (int i = 0; i < TOTAL_SEMS; i++) {
        if (strlen(semaphores[i].semId) > 0) {
            strcat(buffer, semaphores[i].semId);
            for (int j = 0; j < 30 - strlen(semaphores[i].semId); j++) {
                strcat(buffer, " ");
            }
            itoaTruncate(semaphores[i].value, aux, 64);
            strcat(buffer, aux);
            for (int j = 0; j < 9 - numlen(semaphores[i].value); j++) {
                strcat(buffer, " ");
            }
            toBegin(semaphores[i].blockedProcesses);
            while (hasNext(semaphores[i].blockedProcesses)) {
                itoaTruncate(next(semaphores[i].blockedProcesses), aux, 64);
                strcat(buffer, aux);
                strcat(buffer, ", ");
            }
            strcat(buffer, "\n");
        }
    }

    return 0;
}