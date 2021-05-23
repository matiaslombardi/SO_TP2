#ifndef APPS_H
#define APPS_H

#include <stdint.h>

#define PROGRAMS 14

typedef struct {
    char name[12];

    void (*f)(int, char [][25]);

    char description[200];
} programs;

extern programs commands[];

void infoReg(int args, char argv[][25]);

void printMem(int args, char argv[][25]);

void time(int args, char argv[][25]);

void showApps(int args, char argv[][25]);

void chess(int args, char argv[][25]);

void clear(int args, char argv[][25]);

void throwDivZero();

void throwInvOpCode();

void about();

void ps();

void greet();

void loop();

void kill(int args, char argv[][25]);

void nice(int args, char argv[][25]);

void block(int args, char argv[][25]);

void mem(int args, char argv[][25]);

#endif