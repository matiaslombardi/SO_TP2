#include <syscalls.h>
#include <types.h>
#include <lib64.h>
#include <string.h>
#include <stdint.h>

#define READ_SYSCALL 0
#define WRITE_SYSCALL 1
#define DRAW_SYSCALL 2
#define CLEAR_SYSCALL 3
#define ELAPSED_TICKS_SYSCALL 4
#define EXIT_SYSCALL 5
#define INFO_REG 6
#define MEM_DUMP 7
#define TIME_SYSCALL 8
#define SET_ALARM 9
#define SCREEN_HEIGHT 10
#define SCREEN_WIDTH 11
#define PS_SYSCALL 12
#define CREATE_PROCESS_SYSCALL 13
#define CHANGE_STATE_SYSCALL 14
#define CHANGE_PRIORITY_SYSCALL 15
#define GET_PID_SYSCALL 16
#define GET_MEM_SYSCALL 17
#define FREE_SYSCALL 18
#define GET_MEM_INFO_SYSCALL 19


int read(char *buffer, int length) {
    return _syscall(READ_SYSCALL, length, buffer);
}

int write(char *buffer, int row, int col, int color) {
    int length = strlen(buffer);
    return _syscall(WRITE_SYSCALL, length, buffer, row, col, color);
}

void draw(int *matrix, int row, int col, int rows, int columns) {
    _syscall(DRAW_SYSCALL, matrix, row, col, rows, columns);
}

void clearScreen() {
    _syscall(CLEAR_SYSCALL);
}

unsigned int getElapsedTicks(){
    return _syscall(ELAPSED_TICKS_SYSCALL);
}

void _exit(){
    _syscall(EXIT_SYSCALL);
}

void getRegisters(uint64_t *registers) {
    _syscall(INFO_REG, registers);
}

void memoryDump(char *dir, char *dump) {
    _syscall(MEM_DUMP, dir, dump);
}

void getTime(date myDate) {
    _syscall(TIME_SYSCALL, myDate);
}

void setAlarm(void (*func)(void), int flag) {
    _syscall(SET_ALARM, func, flag);
}

int getHeight() {
    return _syscall(SCREEN_HEIGHT);
}

int getWidth() {
    return _syscall(SCREEN_WIDTH);
}

void getProcessesList(){
    _syscall(PS_SYSCALL);
}

unsigned int createProcess(uint64_t * entryPoint, int foreground) {
    return _syscall(CREATE_PROCESS_SYSCALL, entryPoint, foreground);
}

void killProcess(unsigned int pid){
    _syscall(CHANGE_STATE_SYSCALL, pid, 0);
}

void changeProcessPriority(unsigned int pid, int priority){
    _syscall(CHANGE_PRIORITY_SYSCALL, pid, priority);
}

void changeProcessState(unsigned int pid){
    _syscall(CHANGE_STATE_SYSCALL, pid, 1);
}

unsigned int getPid(){
    return _syscall(GET_PID_SYSCALL);
}

void * getMem(uint64_t size){
    return (void *) _syscall(GET_MEM_SYSCALL, size);
}

void freeMem(void * ptr){
    _syscall(FREE_SYSCALL, ptr);
}

void getMemInfo(char *info){
    _syscall(GET_MEM_INFO_SYSCALL, info);
}