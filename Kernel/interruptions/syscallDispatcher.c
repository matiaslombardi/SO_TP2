#include <stdint.h>
#include <videoDriver.h>
#include <keyboardDriver.h>
#include <timerDriver.h>
#include <library.h>
#include <time.h>
#include <scheduler.h>
#include <memDump.h>

#define READ_SYSCALL 0
#define WRITE_SYSCALL 1
#define DRAW_SYSCALL 2
#define CLEAR_SYSCALL 3
#define ELAPSED_TICKS_SYSCALL 4
#define EXIT_SYSCALL 5
#define INFO_REG 8
#define MEM_DUMP 9
#define TIME_SYSCALL 10
#define SET_ALARM 11
#define SCREEN_HEIGHT 12
#define SCREEN_WIDTH 13
#define PS_SYSCALL 14
#define CREATE_PROCESS_SYSCALL 15
#define CHANGE_STATE_SYSCALL 16
#define CHANGE_PRIORITY_SYSCALL 17
#define GET_PID_SYSCALL 18
#define MEM_BYTES 32

int readHandler(int length, char *toRead);

int writeHandler(int length, char *toWrite, int row, int col, int color);

int drawHandler(int *matrix, int row, int col, int rows, int columns);

void getTime(date myDate);

void memDumpHandler(char *dir, char *dump);

void infoRegHandler(uint64_t firstP[]);

void clearScreenHandler();

int screenHeightHandler();

int screenWidthHandler();

void psHandler();

unsigned int createProcessHandler(uint64_t * entryPoint, int foreground);

void changeStateHandler(unsigned int pid, int state);

void changePriorityHandler(unsigned int pid, int priority);

unsigned int getPidHandler();

unsigned int getElapsedTicksHandler();

int syscallDispatcher(uint64_t call, uint64_t firstP, uint64_t secondP, uint64_t thirdP, uint64_t fourthP,
                      uint64_t fifthP) {
    switch (call) {
        case READ_SYSCALL:
            return readHandler((int) firstP, (char *) secondP); // 0 read
        case WRITE_SYSCALL:
            return writeHandler((int) firstP, (char *) secondP, (int) thirdP, (int) fourthP, (int) fifthP); // 1 write
        case DRAW_SYSCALL:
            return drawHandler((int *) firstP, (int) secondP, (int) thirdP, (int) fourthP, (int) fifthP);
        case TIME_SYSCALL:
            getTime((date) firstP);
            return 0;
        case INFO_REG:
            infoRegHandler((uint64_t *) firstP);
            return 0;
        case MEM_DUMP:
            memDumpHandler((char *) firstP, (char *) secondP);
            return 0;
        case CLEAR_SYSCALL:
            clearScreenHandler();
            return 0;
        case SET_ALARM:
            addFunc((void *) firstP, secondP);
            return 0;
        case SCREEN_HEIGHT:
            return screenHeightHandler();
        case SCREEN_WIDTH:
            return screenWidthHandler();
        case PS_SYSCALL:
            psHandler();
            return 0;
        case CREATE_PROCESS_SYSCALL:
            return createProcessHandler((uint64_t *) firstP, (int) secondP);
        case CHANGE_STATE_SYSCALL:
            changeStateHandler((unsigned int) firstP, (int) secondP);
            return 0;
        case CHANGE_PRIORITY_SYSCALL:
            changePriorityHandler((unsigned int) firstP, (int) secondP);
            return 0;
        case GET_PID_SYSCALL:
            return getPidHandler();
        case ELAPSED_TICKS_SYSCALL:
            return getElapsedTicksHandler();
        case EXIT_SYSCALL:
            return 0;
        default:
            return -1;
    }
    return -1;
}

int readHandler(int length, char *toRead) {
    return readBuffer(length, toRead);
}

int writeHandler(int length, char *toWrite, int row, int col, int color) {
    return printStringFrom(toWrite, length, row, col, color);
}

int drawHandler(int *matrix, int row, int col, int rows, int columns) {
    return drawMatrix(matrix, row, col, rows, columns);
}

void getTime(date myDate) {
    getLocalDate(myDate);
}

void memDumpHandler(char *dir, char *dump) {
    memDump(dir, dump);
}

void infoRegHandler(uint64_t firstP[]) {
    fillWithRegs(firstP);
}

void clearScreenHandler() {
    clearScreen();
}

int screenHeightHandler() {
    return screenHeight();
}

int screenWidthHandler() {
    return screenWidth();
}

void psHandler(){
    printProcesses();
}

unsigned int createProcessHandler(uint64_t * entryPoint, int foreground){
    return createProcess(entryPoint); //Falta agregar foreground en scheduler
}

void changeStateHandler(unsigned int pid, int state){
    if (state == 0){ // state 0 = kill.
        endProcess(pid);
    } else{
        switchStates(pid);
    }
}

void changePriorityHandler(unsigned int pid, int priority){
    changePriorities(pid,priority);
}

unsigned int getPidHandler(){
    return getPid();
}

unsigned int getElapsedTicksHandler(){
    return ticks_elapsed();
}