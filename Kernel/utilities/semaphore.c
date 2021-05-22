#include <semaphore.h>

#define TOTAL_SEMS 10
#define SEM_ID_SIZE 10

typedef struct sem {
    char semId[SEM_ID_SIZE];
    uint64_t value;
    QueueADT blockedProcesses;
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

    int firstFree = -1;
//    int found = 0;
    int i;
    for (i = 0; i < TOTAL_SEMS && firstFree == -1; i++) {
        if (strlen(semaphores[i].semId) != 0) {
            if (strcmp(semaphores[i].semId, semId) == 0) {
//                found = 1;
                semaphores[i].attached++;
                release(&lock);
                return 1;
            }
        } else if(firstFree == -1) {
            firstFree = i;
        }
    }

    if (firstFree != -1) {
        strcpy(semaphores[firstFree].semId, semId);
        semaphores[i].blockedProcesses = newQueue();
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
            if(semaphores[i].attached == 1){
                freeQueue(semaphores[i].blockedProcesses);
                semaphores[i].semId[0] = 0; //'\0'
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
        if(!isEmpty(semaphores[found].blockedProcesses)){
            int pid = dequeue(semaphores[found].blockedProcesses);
            wakeup(pid);
        }
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
            enqueue(semaphores[found].blockedProcesses, pid);
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
        if (strlen(semaphores[i].semId) > 0 && strcmp(semaphores[i].semId, semId) == 0) {
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