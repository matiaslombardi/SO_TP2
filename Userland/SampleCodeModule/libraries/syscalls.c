// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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
#define INFO_REG_SYSCALL 6
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
#define SEM_OPEN_SYSCALL 20
#define SEM_CLOSE_SYSCALL 21
#define SEM_POST_SYSCALL 22
#define SEM_WAIT_SYSCALL 23
#define GET_SEM_INFO_SYSCALL 24
#define PIPE_OPEN_SYSCALL 25
#define PIPE_CLOSE_SYSCALL 26
#define GET_PIPE_INFO_SYSCALL 27
#define WAIT_PID_SYSCALL 28

int read(unsigned int fd, char *buffer, int length) {
    Params aux = {READ_SYSCALL, fd, length, (uint64_t) buffer, 0, 0, 0, 0};
    return _syscall(&aux);
}

int write(unsigned int fd, char *buffer, int row, int col, int color) {
    uint64_t length = strlen(buffer);
    Params aux = {WRITE_SYSCALL, fd, length, (uint64_t) buffer, row, col, color, 0};
    return _syscall(&aux);
}

void draw(int *matrix, int row, int col, int rows, int columns) {
    Params aux = {DRAW_SYSCALL, (uint64_t) matrix, row, col, rows, columns, 0, 0};
    _syscall(&aux);
}

void clearScreen() {
    Params aux = {CLEAR_SYSCALL, 0, 0, 0, 0, 0, 0, 0};
    _syscall(&aux);
}

unsigned int getElapsedTicks() {
    Params aux = {ELAPSED_TICKS_SYSCALL, 0, 0, 0, 0, 0, 0, 0};
    return _syscall(&aux);
}

void _exit(uint64_t value) {
    Params aux = {EXIT_SYSCALL, 0, 0, 0, 0, 0, 0, 0};
    _syscall(&aux);
}

void getRegisters(uint64_t *registers) {
    Params aux = {INFO_REG_SYSCALL, (uint64_t) registers, 0, 0, 0, 0, 0, 0};
    _syscall(&aux);
}

void memoryDump(char *dir, char *dump) {
    Params aux = {MEM_DUMP, (uint64_t) dir, (uint64_t) dump, 0, 0, 0, 0, 0};
    _syscall(&aux);
}

void getTime(date myDate) {
    Params aux = {TIME_SYSCALL, (uint64_t) myDate, 0, 0, 0, 0, 0, 0};
    _syscall(&aux);
}

void setAlarm(void (*func)(void), int flag) {
    Params aux = {SET_ALARM, (uint64_t) func, flag, 0, 0, 0, 0, 0};
    _syscall(&aux);
}

int getHeight() {
    Params aux = {SCREEN_HEIGHT, 0, 0, 0, 0, 0, 0, 0};
    return _syscall(&aux);
}

int getWidth() {
    Params aux = {SCREEN_WIDTH, 0, 0, 0, 0, 0, 0, 0};
    return _syscall(&aux);
}

void getProcessesList() {
    Params aux = {PS_SYSCALL, 0, 0, 0, 0, 0, 0, 0};
    _syscall(&aux);
}

unsigned int createProcess(uint64_t *entryPoint, int foreground, uint64_t fdIn, uint64_t fdOut, uint64_t first, uint64_t second, uint64_t third) {
    Params aux = {CREATE_PROCESS_SYSCALL, (uint64_t) entryPoint, foreground, fdIn, fdOut, first, second, third};

    return _syscall(&aux);
}

void killProcess(unsigned int pid) {
    Params aux = {CHANGE_STATE_SYSCALL, pid, 0, 0, 0, 0, 0, 0};
    _syscall(&aux);
}

void changeProcessPriority(unsigned int pid, int priority) {
    Params aux = {CHANGE_PRIORITY_SYSCALL, pid, priority, 0, 0, 0, 0, 0};
    _syscall(&aux);
}

void changeProcessState(unsigned int pid) {
    Params aux = {CHANGE_STATE_SYSCALL, pid, 1, 0, 0, 0, 0, 0};
    _syscall(&aux);
}

unsigned int getPid() {
    Params aux = {GET_PID_SYSCALL, 0, 0, 0, 0, 0, 0, 0};

    return _syscall(&aux);
}

void *getMem(uint64_t size) {
    Params aux = {GET_MEM_SYSCALL, size, 0, 0, 0, 0, 0, 0};
    return (void *) _syscall(&aux);
}

void freeMem(void *ptr) {
    Params aux = {FREE_SYSCALL, (uint64_t) ptr, 0, 0, 0, 0, 0, 0};
    _syscall(&aux);
}

void getMemInfo(char *info) {
    Params aux = {GET_MEM_INFO_SYSCALL, (uint64_t) info, 0, 0, 0, 0, 0, 0};
    _syscall(&aux);
}

int semOpen(char *semId, uint64_t initialValue) {
    Params aux = {SEM_OPEN_SYSCALL, (uint64_t) semId, initialValue, 0, 0, 0, 0, 0};
    return _syscall(&aux);
}

int semClose(char *semId) {
    Params aux = {SEM_CLOSE_SYSCALL, (uint64_t) semId, 0, 0, 0, 0, 0, 0};
    return _syscall(&aux);
}

int semPost(char *semId) {
    Params aux = {SEM_POST_SYSCALL, (uint64_t) semId, 0, 0, 0, 0, 0, 0};
    return _syscall(&aux);
}

int semWait(char *semId) {
    Params aux = {SEM_WAIT_SYSCALL, (uint64_t) semId, 0, 0, 0, 0, 0, 0};
    return _syscall(&aux);
}

int semInfo(char *buffer) {
    Params aux = {GET_SEM_INFO_SYSCALL, (uint64_t) buffer, 0, 0, 0, 0, 0, 0};
    return _syscall(&aux);
}


int pipeOpen(int fd[2]) {
    Params aux = {PIPE_OPEN_SYSCALL, (uint64_t) fd, 0, 0, 0, 0, 0, 0};
    return _syscall(&aux);
}

int pipeClose(int fd) {
    Params aux = {PIPE_CLOSE_SYSCALL, (uint64_t) fd, 0, 0, 0, 0, 0, 0};
    return _syscall(&aux);
}

int pipeInfo(char *buffer) {
    Params aux = {GET_PIPE_INFO_SYSCALL, (uint64_t) buffer, 0, 0, 0, 0, 0, 0};
    return _syscall(&aux);
}

int waitPid(int pid){
    Params aux = {WAIT_PID_SYSCALL, (uint64_t) pid, 0, 0, 0, 0, 0, 0};
    return _syscall(&aux);
}