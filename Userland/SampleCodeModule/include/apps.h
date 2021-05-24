#ifndef APPS_H
#define APPS_H

#include <stdint.h>

#define PROGRAMS 26

typedef struct {
    char name[12];

    int (*f)(int, char [][25]);

    char description[200];

    int canBeBg;
} programs;

extern programs commands[];

int infoReg(int args, char argv[][25]);

int printMem(int args, char argv[][25]);

int time(int args, char argv[][25]);

int showApps(int args, char argv[][25]);

int chess(int args, char argv[][25]);

int clear(int args, char argv[][25]);

int throwDivZero();

int throwInvOpCode();

int about();

int ps();

int greet();

int loop(int args, char argv[][25]);

int kill(int args, char argv[][25]);

int nice(int args, char argv[][25]);

int block(int args, char argv[][25]);

int mem(int args, char argv[][25]);

int sem(int args, char argv[][25]);

int pipe(int args, char argv[][25]);

int catWrapper(int args, char argv[][25]);

int cat(int args, char argv[][25]);

int wcWrapper(int args, char argv[][25]);

int wc(int args, char argv[][25]);

int filterWrapper(int args, char argv[][25]);

int filter(int args, char argv[][25]);

int phylos();

int testMM();

int testPRIO();

int testPROC();

int testSYNC();

int testNoSYNC();

#endif