#include <syscallDispatcher.h>

f syscalls[]={readHandler, writeHandler, drawHandler, clearScreenHandler, getElapsedTicksHandler, getPidHandler, infoRegHandler, memDumpHandler, getTime, addFunc, screenHeightHandler, screenWidthHandler,
              psHandler, createProcessHandler, changeStateHandler, changePriorityHandler, getPidHandler};

int syscallDispatcher(uint64_t call, uint64_t firstP, uint64_t secondP, uint64_t thirdP, uint64_t fourthP,
                      uint64_t fifthP) {
    return syscalls[call](firstP,secondP,thirdP,fourthP,fifthP);

}

int readHandler(uint64_t length, uint64_t toRead) {
    return readBuffer((int)length,(char*) toRead);
}

int writeHandler(uint64_t length,uint64_t toWrite, uint64_t row, uint64_t col, uint64_t color) {
    return printStringFrom((char*)toWrite, (int)length, (int)row, (int)col, (int)color);
}

int drawHandler(uint64_t matrix, uint64_t row, uint64_t col, uint64_t rows, uint64_t columns) {
    return drawMatrix((int*)matrix, (int)row, (int)col, (int)rows, (int)columns);
}

int getTime(uint64_t myDate) {
    getLocalDate((date)myDate);
    return 0;
}

int memDumpHandler(uint64_t dir, uint64_t dump) {
    memDump((char*)dir, (char*)dump);
    return 0;
}

int infoRegHandler(uint64_t firstP) {
    fillWithRegs((uint64_t*)firstP);
    return 0;
}

int clearScreenHandler() {
    clearScreen();
    return 0;
}

int screenHeightHandler() {
    return screenHeight();
}

int screenWidthHandler() {
    return screenWidth();
}

int psHandler(){
    printProcesses();
    return 0;
}

int createProcessHandler(uint64_t entryPoint, uint64_t foreground){
    return createProcess((uint64_t*)entryPoint); //Falta agregar foreground en scheduler
}

int changeStateHandler(uint64_t pid, uint64_t state){
    if (state == 0){ // state 0 = kill.
        endProcess((unsigned int)pid);
    } else{
        switchStates((unsigned int)pid);
    }
}

int changePriorityHandler(uint64_t pid, uint64_t priority){
    changePriorities((unsigned int)pid, (unsigned int)priority);
    return 0;
}

int getPidHandler(){
    return getPid();
}

int getElapsedTicksHandler(){
    return ticks_elapsed();
}