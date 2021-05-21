#include <semaphore.h>

#define TOTAL_SEMS 10
#define SEM_ID_SIZE 10

typedef struct sem {
    char semId[SEM_ID_SIZE];
    uint64_t value;
    uint64_t blockedPid;
    uint64_t attached;
} sem;

static sem semaphores[TOTAL_SEMS];

static int searchSem(char * semId);
static void acquire(int * lock);
static void release(int * lock);

int lock = 0;

int semOpen(char *semId, uint64_t initialValue) {
    int len = strlen(semId);
    if (len < 0 || len > SEM_ID_SIZE - 1) {
        return 0;
    }

    acquire(&lock);

    int lastFree = -1;
    int found = 0;
    int i;
    for (i = 0; i < TOTAL_SEMS && !found; i++) {
        if (strlen(semaphores[i].semId) != 0) {
            if (strcmp(semaphores[i].semId, semId) == 0) {
                found = 1;
                semaphores[i].attached++;
                release(&lock);
                return 1;
            }
        } else {
            lastFree = i;
        }
    }

    if (!found && lastFree != -1) {
        strcpy(semaphores[lastFree].semId, semId);
        semaphores[lastFree].value = initialValue;
        semaphores[lastFree].attached = 1;
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
            if(semaphores[i].attached == 1){
                semaphores[i].semId[0] = 0;
            } else{
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

    if(found != -1) {
        semaphores[found].value++;
        wakeup(semaphores[found].blockedPid);
        release(&lock);
        return 1;
    }

    release(&lock);
    return 0;
}

int semWait(char * semId) {
    acquire(&lock);
    int found = searchSem(semId);

    if(found != -1) {
        if(semaphores[found].value > 0) {
            semaphores[found].value--;
            release(&lock);
        } else {
            uint64_t pid = getPid();
            semaphores[found].blockedPid = pid;
            release(&lock);
            sleep(pid);
            semaphores[found].value--;
        }
        return 1;
    }
    release(&lock);
    return 0;
}

static int searchSem(char * semId) {
    for (int i = 0; i < TOTAL_SEMS; i++) {
        if (strcmp(semaphores[i].semId, semId) == 0) {
            return i;
        }
    }
    return -1;
}

static void acquire(int * lock) {
    while(_xchg(lock, 1) != 0);
}

static void release(int * lock) {
    _xchg(lock, 0);
}