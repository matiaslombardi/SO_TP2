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

int semOpen(char *semId, uint64_t initialValue) {
    int len = strlen(semId);
    if (len < 0 || len > SEM_ID_SIZE - 1) {
        return 0;
    }

    int lastFree = -1;
    int found = 0;
    int i;
    for (i = 0; i < TOTAL_SEMS && !found; i++) {
        if (strlen(semaphores[i].semId) != 0) {
            if (strcmp(semaphores[i].semId, semId) == 0) {
                found = 1;
                semaphores[i].attached++;
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
        return 1;
    }

    return 0;
}

int semClose(char *semId) {
    for (int i = 0; i < TOTAL_SEMS; i++) {
        if (strcmp(semaphores[i].semId, semId) == 0) {
            if(semaphores[i].attached == 1){
                semaphores[i].semId[0] = 0;
            } else{
                semaphores[i].attached--;
            }
            return 1;
        }
    }
    return 0;
}

int semPost(char *semId) {
    int found = searchSem(semId);

    if(found != -1) {
        semaphores[found].value++;
        wakeup(semaphores[found].blockedPid);
        return 1;
    }

    return 0;
}

int semWait(char * semId) {
    int found = searchSem(semId);

    if(found != -1) {
        if(semaphores[found].value > 0) {
            semaphores[found].value--;
        } else {
            uint64_t pid = getPid();
            semaphores[found].blockedPid = pid;
            sleep(pid);
            semaphores[found].value--;
        }
        return 1;
    }

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