// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <syscallDispatcher.h>
#include <scheduler.h>

typedef struct Params {
    uint64_t call;
    uint64_t first;
    uint64_t second;
    uint64_t third;
    uint64_t fourth;
    uint64_t fifth;
    uint64_t sixth;
    uint64_t seventh;
    uint64_t eighth;
} Params;

static syscall_t syscallsArray[] = {readHandler, writeHandler, drawHandler, clearScreenHandler,
                                    getElapsedTicksHandler, exitHandler, infoRegHandler, memDumpHandler,
                                    getTime, setAlarmHandler, screenHeightHandler, screenWidthHandler,
                                    psHandler, createProcessHandler, changeStateHandler,
                                    changePriorityHandler, getPidHandler, getMemHandler, freeMemHandler,
                                    getMemInfoHandler, semOpenHandler, semCloseHandler, semPostHandler,
                                    semWaitHandler, fillSemInfoHandler, pipeOpenHandler, pipeCloseHandler,
                                    fillPipeInfoHandler, waitPidHandler, resignCpuHandler};

uint64_t syscallDispatcher(Params *p) {
    return syscallsArray[p->call](p->first, p->second, p->third, p->fourth, p->fifth, p->sixth, p->seventh, p->eighth);
}

uint64_t readHandler(uint64_t fd, uint64_t length, uint64_t toRead, uint64_t fourthP, uint64_t fifthP, uint64_t sixthP,
                     uint64_t seventhP, uint64_t eighthP) {
    if (fd == STDIN) {
        if (getFdIn() == STDIN) {
            if (!getFg()) {
                exit();
            }
            return readBuffer(length, (char *) toRead);
        } else {
            return pipeRead(getFdIn(), length, (char *) toRead);
        }
    }
    return pipeRead(fd, length, (char *) toRead);
}

uint64_t writeHandler(uint64_t fd, uint64_t length, uint64_t toWrite, uint64_t row, uint64_t col, uint64_t color,
                      uint64_t seventhP, uint64_t eighthP) {
    if (fd == STDOUT) {
        if (getFdOut() == STDOUT) {
            return printStringFrom((char *) toWrite, (int) length, (int) row, (int) col, (int) color);
        } else {
            return pipeWrite(getFdOut(), length, (char *) toWrite);
        }
    }
    return pipeWrite(fd, length, (char *) toWrite);
}

uint64_t drawHandler(uint64_t matrix, uint64_t row, uint64_t col, uint64_t rows, uint64_t columns, uint64_t sixthP,
                     uint64_t seventhP, uint64_t eighthP) {
    return drawMatrix((int *) matrix, (int) row, (int) col, (int) rows, (int) columns);
}

uint64_t clearScreenHandler(uint64_t firstP, uint64_t secondP, uint64_t thirdP, uint64_t fourthP,
                            uint64_t fifthP, uint64_t sixthP, uint64_t seventhP, uint64_t eighthP) {
    clearScreen();
    return 0;
}

uint64_t getElapsedTicksHandler(uint64_t firstP, uint64_t secondP, uint64_t thirdP, uint64_t fourthP,
                                uint64_t fifthP, uint64_t sixthP, uint64_t seventhP, uint64_t eighthP) {
    return ticks_elapsed();
}

uint64_t
exitHandler(uint64_t firstP, uint64_t secondP, uint64_t thirdP, uint64_t fourthP, uint64_t fifthP, uint64_t sixthP,
            uint64_t seventhP, uint64_t eighthP) {
    exit();
    return 0;
}


uint64_t infoRegHandler(uint64_t firstP, uint64_t secondP, uint64_t thirdP, uint64_t fourthP,
                        uint64_t fifthP, uint64_t sixthP, uint64_t seventhP, uint64_t eighthP) {
    fillWithRegs((uint64_t *) firstP);
    return 0;
}

uint64_t
memDumpHandler(uint64_t dir, uint64_t dump, uint64_t thirdP, uint64_t fourthP, uint64_t fifthP, uint64_t sixthP,
               uint64_t seventhP, uint64_t eighthP) {
    memDump((char *) dir, (char *) dump);
    return 0;
}

uint64_t getTime(uint64_t myDate, uint64_t secondP, uint64_t thirdP, uint64_t fourthP, uint64_t fifthP, uint64_t sixthP,
                 uint64_t seventhP, uint64_t eighthP) {
    getLocalDate((date) myDate);
    return 0;
}

uint64_t
setAlarmHandler(uint64_t function, uint64_t flag, uint64_t thirdP, uint64_t fourthP, uint64_t fifthP, uint64_t sixthP,
                uint64_t seventhP, uint64_t eighthP) {
    addFunc((void (*)(void)) function, (int) flag);
    return 0;
}

uint64_t screenHeightHandler(uint64_t firstP, uint64_t secondP, uint64_t thirdP, uint64_t fourthP,
                             uint64_t fifthP, uint64_t sixthP, uint64_t seventhP, uint64_t eighthP) {
    return screenHeight();
}

uint64_t screenWidthHandler(uint64_t firstP, uint64_t secondP, uint64_t thirdP, uint64_t fourthP,
                            uint64_t fifthP, uint64_t sixthP, uint64_t seventhP, uint64_t eighthP) {
    return screenWidth();
}

uint64_t
psHandler(uint64_t firstP, uint64_t secondP, uint64_t thirdP, uint64_t fourthP, uint64_t fifthP, uint64_t sixthP,
          uint64_t seventhP, uint64_t eighthP) {
    printProcesses();
    return 0;
}

uint64_t createProcessHandler(uint64_t entryPoint, uint64_t foreground, uint64_t fdIn, uint64_t fdOut,
                              uint64_t fifthP, uint64_t sixthP, uint64_t seventhP, uint64_t name) {
    return createProcess((uint64_t *) entryPoint, foreground, fdIn, fdOut, fifthP, sixthP, seventhP, (char *) name);
}

uint64_t
changeStateHandler(uint64_t pid, uint64_t state, uint64_t thirdP, uint64_t fourthP, uint64_t fifthP, uint64_t sixthP,
                   uint64_t seventhP, uint64_t eighthP) {
    if (state == 0) { // state 0 = kill.
        endProcess((unsigned int) pid);
    } else {
        switchStates((unsigned int) pid);
    }
    return 0;
}

uint64_t changePriorityHandler(uint64_t pid, uint64_t priority, uint64_t thirdP, uint64_t fourthP,
                               uint64_t fifthP, uint64_t sixthP, uint64_t seventhP, uint64_t eighthP) {
    changePriorities((unsigned int) pid, (unsigned int) priority);
    return 0;
}

uint64_t
getPidHandler(uint64_t firstP, uint64_t secondP, uint64_t thirdP, uint64_t fourthP, uint64_t fifthP, uint64_t sixthP,
              uint64_t seventhP, uint64_t eighthP) {
    return getPid();
}

uint64_t
getMemHandler(uint64_t size, uint64_t secondP, uint64_t thirdP, uint64_t fourthP, uint64_t fifthP, uint64_t sixthP,
              uint64_t seventhP, uint64_t eighthP) {
    return (uint64_t) mmMalloc(size);
}

uint64_t
freeMemHandler(uint64_t ptr, uint64_t secondP, uint64_t thirdP, uint64_t fourthP, uint64_t fifthP, uint64_t sixthP,
               uint64_t seventhP, uint64_t eighthP) {
    mmFree((void *) ptr);
    return 0;
}

uint64_t getMemInfoHandler(uint64_t toWrite, uint64_t secondP, uint64_t thirdP, uint64_t fourthP,
                           uint64_t fifthP, uint64_t sixthP, uint64_t seventhP, uint64_t eighthP) {
    fillMemInfo((char *) toWrite);
    return 0;
}

uint64_t semOpenHandler(uint64_t semId, uint64_t initialValue, uint64_t thirdP, uint64_t fourthP,
                        uint64_t fifthP, uint64_t sixthP, uint64_t seventhP, uint64_t eighthP) {
    return semOpen((char *) semId, initialValue);
}

uint64_t semCloseHandler(uint64_t semId, uint64_t secondP, uint64_t thirdP, uint64_t fourthP,
                         uint64_t fifthP, uint64_t sixthP, uint64_t seventhP, uint64_t eighthP) {
    return semClose((char *) semId);
}

uint64_t semPostHandler(uint64_t semId, uint64_t secondP, uint64_t thirdP, uint64_t fourthP,
                        uint64_t fifthP, uint64_t sixthP, uint64_t seventhP, uint64_t eighthP) {
    return semPost((char *) semId);
}

uint64_t semWaitHandler(uint64_t semId, uint64_t secondP, uint64_t thirdP, uint64_t fourthP,
                        uint64_t fifthP, uint64_t sixthP, uint64_t seventhP, uint64_t eighthP) {
    return semWait((char *) semId);
}

uint64_t fillSemInfoHandler(uint64_t buffer, uint64_t secondP, uint64_t thirdP, uint64_t fourthP,
                            uint64_t fifthP, uint64_t sixthP, uint64_t seventhP, uint64_t eighthP) {
    return fillSemInfo((char *) buffer);
}

uint64_t pipeOpenHandler(uint64_t fd, uint64_t secondP, uint64_t thirdP, uint64_t fourthP,
                         uint64_t fifthP, uint64_t sixthP, uint64_t seventhP, uint64_t eighthP) {
    return pipeOpen((int *) fd);
}

uint64_t pipeCloseHandler(uint64_t fd, uint64_t secondP, uint64_t thirdP, uint64_t fourthP,
                          uint64_t fifthP, uint64_t sixthP, uint64_t seventhP, uint64_t eighthP) {
    return pipeClose((int) fd);
}

uint64_t fillPipeInfoHandler(uint64_t buffer, uint64_t secondP, uint64_t thirdP, uint64_t fourthP,
                             uint64_t fifthP, uint64_t sixthP, uint64_t seventhP, uint64_t eighthP) {
    return fillPipeInfo((char *) buffer);
}


uint64_t waitPidHandler(uint64_t pid, uint64_t secondP, uint64_t thirdP, uint64_t fourthP,
                        uint64_t fifthP, uint64_t sixthP, uint64_t seventhP, uint64_t eighthP) {
    return addWaitingPid(pid);
}

uint64_t resignCpuHandler(uint64_t firstP, uint64_t secondP, uint64_t thirdP, uint64_t fourthP, uint64_t fifthP, uint64_t sixthP,
              uint64_t seventhP, uint64_t eighthP) {
    resignCpu();
    return 0;
}