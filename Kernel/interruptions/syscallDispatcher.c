#include <syscallDispatcher.h>
//
//uint64_t* (*syscalls[])(uint64_t, ...) = {readHandler, writeHandler, drawHandler, clearScreenHandler, getElapsedTicksHandler, getPidHandler, infoRegHandler, memDumpHandler, getTime, addFunc, screenHeightHandler, screenWidthHandler,
//              psHandler, createProcessHandler, changeStateHandler, changePriorityHandler, getPidHandler, getMemHandler, freeMemHandler, getMemInfoHandler};
//
//f syscalls[]= {readHandler, writeHandler, drawHandler, clearScreenHandler, getElapsedTicksHandler, getPidHandler, infoRegHandler, memDumpHandler, getTime, addFunc, screenHeightHandler, screenWidthHandler,
//             psHandler, createProcessHandler, changeStateHandler, changePriorityHandler, getPidHandler, getMemHandler, freeMemHandler, getMemInfoHandler};

static syscall_t syscallsArray[] = {readHandler, writeHandler, drawHandler, clearScreenHandler, getElapsedTicksHandler, getPidHandler, infoRegHandler, memDumpHandler, getTime, setAlarmHandler, screenHeightHandler, screenWidthHandler,
             psHandler, createProcessHandler, changeStateHandler, changePriorityHandler, getPidHandler, getMemHandler, freeMemHandler, getMemInfoHandler};

uint64_t syscallDispatcher(uint64_t call, uint64_t firstP, uint64_t secondP, uint64_t thirdP,
                           uint64_t fourthP, uint64_t fifthP) {
    return syscallsArray[call](firstP, secondP, thirdP, fourthP, fifthP);
}

uint64_t readHandler(uint64_t length, uint64_t toRead, uint64_t third, uint64_t fourthP, uint64_t fifthP) {
    return readBuffer((int)length,(char*) toRead);
}

uint64_t writeHandler(uint64_t length,uint64_t toWrite, uint64_t row, uint64_t col, uint64_t color) {
    return printStringFrom((char*)toWrite, (int)length, (int)row, (int)col, (int)color);
}

uint64_t drawHandler(uint64_t matrix, uint64_t row, uint64_t col, uint64_t rows, uint64_t columns) {
    return drawMatrix((int*)matrix, (int)row, (int)col, (int)rows, (int)columns);
}

uint64_t clearScreenHandler(uint64_t firstP, uint64_t secondP, uint64_t thirdP,
                            uint64_t fourthP, uint64_t fifthP) {
    clearScreen();
    return 0;
}

uint64_t getElapsedTicksHandler(uint64_t firstP, uint64_t secondP, uint64_t thirdP,
                                uint64_t fourthP, uint64_t fifthP){
    return ticks_elapsed();
}

/*
 * exit
 */


uint64_t infoRegHandler(uint64_t firstP, uint64_t secondP, uint64_t thirdP,
                        uint64_t fourthP, uint64_t fifthP) {
    fillWithRegs((uint64_t*)firstP);
    return 0;
}

uint64_t memDumpHandler(uint64_t dir, uint64_t dump, uint64_t thirdP,
                        uint64_t fourthP, uint64_t fifthP) {
    memDump((char*)dir, (char*)dump);
    return 0;
}

uint64_t getTime(uint64_t myDate, uint64_t secondP, uint64_t thirdP,
                 uint64_t fourthP, uint64_t fifthP) {
    getLocalDate((date)myDate);
    return 0;
}

uint64_t setAlarmHandler(uint64_t function, uint64_t flag, uint64_t thirdP,
                         uint64_t fourthP, uint64_t fifthP){
    addFunc((void (*) (void))function, (int)flag);
    return 0;
}

uint64_t screenHeightHandler(uint64_t firstP, uint64_t secondP, uint64_t thirdP,
                             uint64_t fourthP, uint64_t fifthP) {
    return screenHeight();
}

uint64_t screenWidthHandler(uint64_t firstP, uint64_t secondP, uint64_t thirdP,
                            uint64_t fourthP, uint64_t fifthP) {
    return screenWidth();
}

uint64_t psHandler(uint64_t firstP, uint64_t secondP, uint64_t thirdP,
                   uint64_t fourthP, uint64_t fifthP){
    printProcesses();
    return 0;
}

uint64_t createProcessHandler(uint64_t entryPoint, uint64_t foreground, uint64_t thirdP,
                              uint64_t fourthP, uint64_t fifthP){
    return createProcess((uint64_t*)entryPoint); //Falta agregar foreground en scheduler
}

uint64_t changeStateHandler(uint64_t pid, uint64_t state, uint64_t thirdP,
                            uint64_t fourthP, uint64_t fifthP){
    if (state == 0){ // state 0 = kill.
        endProcess((unsigned int)pid);
    } else{
        switchStates((unsigned int)pid);
    }
}

uint64_t changePriorityHandler(uint64_t pid, uint64_t priority, uint64_t thirdP,
                               uint64_t fourthP, uint64_t fifthP){
    changePriorities((unsigned int)pid, (unsigned int)priority);
    return 0;
}

uint64_t getPidHandler(uint64_t firstP, uint64_t secondP, uint64_t thirdP,
                       uint64_t fourthP, uint64_t fifthP){
    return getPid();
}

uint64_t getMemHandler(uint64_t size, uint64_t secondP, uint64_t thirdP,
                       uint64_t fourthP, uint64_t fifthP){
    return mmMalloc(size);
}

uint64_t freeMemHandler(uint64_t ptr, uint64_t secondP, uint64_t thirdP,
                        uint64_t fourthP, uint64_t fifthP) {
    mmFree((void*)ptr);
    return 0;
}

uint64_t getMemInfoHandler(uint64_t toWrite, uint64_t secondP, uint64_t thirdP,
                           uint64_t fourthP, uint64_t fifthP) {
    fillMemInfo(toWrite);
    return 0;
}
