#include <syscallDispatcher.h>

typedef struct Params{
    uint64_t call;
    uint64_t first;
    uint64_t second;
    uint64_t third;
    uint64_t fourth;
    uint64_t fifth;
    uint64_t sixth;
    uint64_t seventh;
} Params;

static syscall_t syscallsArray[] = {readHandler, writeHandler, drawHandler, clearScreenHandler,
                                    getElapsedTicksHandler, exitHandler, infoRegHandler, memDumpHandler,
                                    getTime, setAlarmHandler, screenHeightHandler, screenWidthHandler,
                                    psHandler, createProcessHandler, changeStateHandler,
                                    changePriorityHandler, getPidHandler, getMemHandler, freeMemHandler,
                                    getMemInfoHandler, semOpenHandler, semCloseHandler, semPostHandler,
                                    semWaitHandler};

uint64_t syscallDispatcher(Params* p) {
    return syscallsArray[p->call](p->first, p->second, p->third, p->fourth, p->fifth, p->sixth, p->seventh);
}

uint64_t readHandler(uint64_t fd, uint64_t length, uint64_t toRead, uint64_t fourthP, uint64_t fifthP, uint64_t sixthP, uint64_t seventhP) {
    if(fd == STDIN){
        if(getFdIn() == STDIN){ //read from keyboard
            return readBuffer((int) length, (char *) toRead);
        } else {

        }
    }
    return readBuffer((int) length, (char *) toRead);
}

uint64_t writeHandler(uint64_t fd, uint64_t length, uint64_t toWrite, uint64_t row, uint64_t col, uint64_t color, uint64_t seventhP) {
    if(fd == STDOUT){
        if(getFdOut() == STDOUT){ //write to screen
            return printStringFrom((char *) toWrite, (int) length, (int) row, (int) col, (int) color);
        } else {
            println("//TODO");
            //TODO
        }
    }
    return printStringFrom((char *) toWrite, (int) length, (int) row, (int) col, (int) color);
}

uint64_t drawHandler(uint64_t matrix, uint64_t row, uint64_t col, uint64_t rows, uint64_t columns, uint64_t sixthP, uint64_t seventhP) {
    return drawMatrix((int *) matrix, (int) row, (int) col, (int) rows, (int) columns);
}

uint64_t clearScreenHandler(uint64_t firstP, uint64_t secondP, uint64_t thirdP, uint64_t fourthP,
                            uint64_t fifthP, uint64_t sixthP, uint64_t seventhP) {
    clearScreen();
    return 0;
}

uint64_t getElapsedTicksHandler(uint64_t firstP, uint64_t secondP, uint64_t thirdP, uint64_t fourthP,
                                uint64_t fifthP, uint64_t sixthP, uint64_t seventhP) {
    return ticks_elapsed();
}

uint64_t exitHandler(uint64_t firstP, uint64_t secondP, uint64_t thirdP, uint64_t fourthP, uint64_t fifthP, uint64_t sixthP, uint64_t seventhP) {
    exit();
    return 0;
}


uint64_t infoRegHandler(uint64_t firstP, uint64_t secondP, uint64_t thirdP, uint64_t fourthP,
                        uint64_t fifthP, uint64_t sixthP, uint64_t seventhP) {
    fillWithRegs((uint64_t *) firstP);
    return 0;
}

uint64_t memDumpHandler(uint64_t dir, uint64_t dump, uint64_t thirdP, uint64_t fourthP, uint64_t fifthP, uint64_t sixthP, uint64_t seventhP) {
    memDump((char *) dir, (char *) dump);
    return 0;
}

uint64_t getTime(uint64_t myDate, uint64_t secondP, uint64_t thirdP, uint64_t fourthP, uint64_t fifthP, uint64_t sixthP, uint64_t seventhP) {
    getLocalDate((date) myDate);
    return 0;
}

uint64_t setAlarmHandler(uint64_t function, uint64_t flag, uint64_t thirdP, uint64_t fourthP, uint64_t fifthP, uint64_t sixthP, uint64_t seventhP) {
    addFunc((void (*)(void)) function, (int) flag);
    return 0;
}

uint64_t screenHeightHandler(uint64_t firstP, uint64_t secondP, uint64_t thirdP, uint64_t fourthP,
                             uint64_t fifthP, uint64_t sixthP, uint64_t seventhP) {
    return screenHeight();
}

uint64_t screenWidthHandler(uint64_t firstP, uint64_t secondP, uint64_t thirdP, uint64_t fourthP,
                            uint64_t fifthP, uint64_t sixthP, uint64_t seventhP) {
    return screenWidth();
}

uint64_t psHandler(uint64_t firstP, uint64_t secondP, uint64_t thirdP, uint64_t fourthP, uint64_t fifthP, uint64_t sixthP, uint64_t seventhP) {
    printProcesses();
    return 0;
}

uint64_t createProcessHandler(uint64_t entryPoint, uint64_t foreground, uint64_t fdIn, uint64_t fdOut,
                              uint64_t fifthP, uint64_t sixthP, uint64_t seventhP) {
    return createProcess((uint64_t *) entryPoint, foreground, fdIn, fdOut, fifthP, sixthP, seventhP); //Falta agregar foreground en scheduler
}

uint64_t changeStateHandler(uint64_t pid, uint64_t state, uint64_t thirdP, uint64_t fourthP, uint64_t fifthP, uint64_t sixthP, uint64_t seventhP) {
    if (state == 0) { // state 0 = kill.
        endProcess((unsigned int) pid);
    } else {
        switchStates((unsigned int) pid);
    }
    return 0; //TODO retorne algo quizas
}

uint64_t changePriorityHandler(uint64_t pid, uint64_t priority, uint64_t thirdP, uint64_t fourthP,
                               uint64_t fifthP, uint64_t sixthP, uint64_t seventhP) {
    changePriorities((unsigned int) pid, (unsigned int) priority);
    return 0;
}

uint64_t getPidHandler(uint64_t firstP, uint64_t secondP, uint64_t thirdP, uint64_t fourthP, uint64_t fifthP, uint64_t sixthP, uint64_t seventhP) {
    return getPid();
}

uint64_t getMemHandler(uint64_t size, uint64_t secondP, uint64_t thirdP, uint64_t fourthP, uint64_t fifthP, uint64_t sixthP, uint64_t seventhP) {
    return (uint64_t) mmMalloc(size);
}

uint64_t freeMemHandler(uint64_t ptr, uint64_t secondP, uint64_t thirdP, uint64_t fourthP, uint64_t fifthP, uint64_t sixthP, uint64_t seventhP) {
    mmFree((void *) ptr);
    return 0;
}

uint64_t getMemInfoHandler(uint64_t toWrite, uint64_t secondP, uint64_t thirdP, uint64_t fourthP,
                           uint64_t fifthP, uint64_t sixthP, uint64_t seventhP) {
    fillMemInfo((char *) toWrite);
    return 0;
}

uint64_t semOpenHandler(uint64_t semId, uint64_t initialValue, uint64_t thirdP, uint64_t fourthP,
                        uint64_t fifthP, uint64_t sixthP, uint64_t seventhP) {
    return semOpen((char *) semId, initialValue);
}

uint64_t semCloseHandler(uint64_t semId, uint64_t secondP, uint64_t thirdP, uint64_t fourthP,
                         uint64_t fifthP, uint64_t sixthP, uint64_t seventhP) {
    return semClose((char *) semId);
}

uint64_t semPostHandler(uint64_t semId, uint64_t secondP, uint64_t thirdP, uint64_t fourthP,
                        uint64_t fifthP, uint64_t sixthP, uint64_t seventhP) {
    return semPost((char *) semId);
}

uint64_t semWaitHandler(uint64_t semId, uint64_t secondP, uint64_t thirdP, uint64_t fourthP,
                        uint64_t fifthP, uint64_t sixthP, uint64_t seventhP) {
    return semWait((char *) semId);
}