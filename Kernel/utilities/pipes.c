// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <pipes.h>

#define TOTAL_PIPES 10
#define MAX_BUFFER 256

typedef struct Pipe {
    unsigned int fdIn;
    unsigned int fdOut;
    char buffer[MAX_BUFFER];
    unsigned int readPos;
    unsigned int writePos;
    QueueADT blockedProcesses;
    uint64_t attached;
    uint64_t isActive;
    uint64_t inClosed;
    uint64_t outClosed;

} Pipe;

static Pipe pipes[TOTAL_PIPES];

static int openedPipes = 0;

static int nextFd = 3;

static int searchPipe(int fd);

/*
 * Convención: fd[0]: lectura | fd[1]: escritura
 * */
int pipeOpen(int fd[2]) {
    if (openedPipes == TOTAL_PIPES - 1) {
        return -1;
    }

    int i;
    for (i = 0; i < TOTAL_PIPES; i++) {
        if (!pipes[i].isActive) {
            break;
        }
    }
    if (i == TOTAL_PIPES) {
        return -1;
    }

    pipes[i].fdIn = nextFd++;
    pipes[i].fdOut = nextFd++;

    pipes[i].inClosed = 0;
    pipes[i].outClosed = 0;

    pipes[i].readPos = 0;
    pipes[i].writePos = 0;

    pipes[i].blockedProcesses = newQueue();

    pipes[i].isActive = 1;
    openedPipes++;

    fd[0] = pipes[i].fdIn;
    fd[1] = pipes[i].fdOut;
    return 0;
}

int pipeClose(int fd) {
    int index = searchPipe(fd);
    if (index != -1) {
        if (pipes[index].fdIn == fd) {
            pipes[index].inClosed = 1;
        }
        if (pipes[index].fdOut == fd) {
            pipes[index].outClosed = 1;
            if (!isEmpty(pipes[index].blockedProcesses)) {
                wakeup(dequeue(pipes[index].blockedProcesses));
            }
        }

        if (pipes[index].inClosed) {
            freeQueue(pipes[index].blockedProcesses);
            openedPipes--;
            pipes[index].isActive = 0;
        }
        return 0;
    }
    return -1;
}

int pipeRead(int fd, int length, char *buffer) {
    int index = searchPipe(fd);
    if (index == -1 || pipes[index].fdOut == fd || pipes[index].inClosed) {
        return 0;
    }

    Pipe *aux = &pipes[index];
    if (aux->readPos == aux->writePos) {
        if (aux->outClosed) {
            return -1; //EOF
        }
        int pid = getPid();
        enqueue(aux->blockedProcesses, pid);
        sleep(pid);
    }

    int i;
    for (i = 0; aux->readPos != aux->writePos && i < length; i++) {
        buffer[i] = aux->buffer[aux->readPos];
        aux->buffer[aux->readPos] = 0;
        aux->readPos = (aux->readPos + 1) % MAX_BUFFER;
    }

    buffer[i] = 0;
    return i;
}

int pipeWrite(int fd, int length, char *buffer) {
    int index = searchPipe(fd);
    if (index == -1 || pipes[index].fdIn == fd || pipes[index].outClosed) {
        return 0;
    }

    Pipe *aux = &pipes[index];

    int i;
    for (i = 0; i < length; i++) {
        aux->buffer[aux->writePos] = buffer[i];
        aux->writePos = (aux->writePos + 1) % MAX_BUFFER;
    }

    if (!isEmpty(aux->blockedProcesses)) {
        wakeup(dequeue(aux->blockedProcesses));
    }
    return i;
}


static int searchPipe(int fd) {
    for (int i = 0; i < TOTAL_PIPES; i++) {
        if (pipes[i].isActive && (pipes[i].fdIn == fd || pipes[i].fdOut == fd)) {
            return i;
        }
    }
    return -1;
}

int fillPipeInfo(char *buffer) {
    char aux[64] = {0};


    strcat(buffer, "FdIn     ");
    strcat(buffer, "FdOut    ");
    strcat(buffer, "Blocked Processes\n");

    for (int i = 0; i < TOTAL_PIPES; i++) {
        if (pipes[i].isActive) {
            itoaTruncate(pipes[i].fdIn, aux, 64);
            strcat(buffer, aux);
            for (int j = 0; j < 9 - numlen(pipes[i].fdIn); j++) {
                strcat(buffer, " ");
            }
            itoaTruncate(pipes[i].fdOut, aux, 64);
            strcat(buffer, aux);
            for (int j = 0; j < 9 - numlen(pipes[i].fdOut); j++) {
                strcat(buffer, " ");
            }
            toBegin(pipes[i].blockedProcesses);
            while (hasNext(pipes[i].blockedProcesses)) {
                itoaTruncate(next(pipes[i].blockedProcesses), aux, 64);
                strcat(buffer, aux);
                strcat(buffer, ", ");
            }
            strcat(buffer, "\n");
        }
    }

    return 0;
}