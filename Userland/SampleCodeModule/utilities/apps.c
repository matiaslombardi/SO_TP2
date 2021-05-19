#include <chessHandler.h>
#include <syscalls.h>
#include <standardIO.h>
#include <types.h>
#include <string.h>
#include <stdint.h>
#include <lib64.h>
#include <apps.h>

#define MEM_BYTES 32
#define BUFFER64_BITS 20

char array[] = {121, 27, 3, 4, 5, 6, 7, 8, 9, 10};

programs commands[] = {{"about",       about,          "      Information about the O.S and authors."},
                       {"help",        showApps,       "       Menu of the differents apps."},
                       {"time",        time,           "       Displays the systems current time."},
                       {"inforeg",     infoReg,        "    Displays the registers current state."},
                       {"printmem",    printMem,       "   Prints on screen the first 32 bytes from a given position."},
                       {"chess",       chess,          "      Starts a PVP chess match. Try chess help for aditional info."},
                       {"clear",       clear,          "      Clears the current screen."},
                       {"ps",          ps,             "         Displays a list of all the running processes with relevant data."},
                       {"loop",        loop,           "       Prints a process ID given a determined number of seconds."},
                       {"kill",        kill,           "       Kills a process given its ID."},
                       {"nice",        nice,           "       Changes the given process priority."},
                       {"block",       block,          "      Changes the process state given its ID."},
                       {"mem",         mem,            "        Displays the current state of the memory."},
                       {"exceptionZ",  throwDivZero,   " Throws a divide by zero exception"},
                       {"exceptionOP", throwInvOpCode, "Throws an invalid Operation Code Exception"}
};

int checkArgs(int args, int expected);

void infoReg(int args, char argv[][25]) {
    if (!checkArgs(args, 0)) return;

    char regis[][4] = {"R15", "R14", "R13", "R12", "R11", "R10", "R9 ", "R8 ", "RSI", "RDI", "RBP", "RDX", "RCX", "RBX",
                       "RAX", "RIP", "CS ", "FLG", "RSP"};
    uint64_t values[REGISTERS_AMOUNT];
    getRegisters(values);
    char buffer[BUFFER64_BITS] = {0};

    for (int i = 0; i < REGISTERS_AMOUNT; i++) {
        turnToBaseN(values[i], 16, buffer, BUFFER64_BITS);
        printc(regis[i], 0xaa08d0);
        printc(":  ", 0xaa08d0);
        print("0x");
        println(buffer);
    }

}

void printMem(int args, char argv[][25]) {
    if (!checkArgs(args, 1)) return;

    char *dir = (char *) stringToInt(argv[1]);
    char dump[MEM_BYTES];
    memoryDump(dir, dump);
    char buffer[BUFFER64_BITS] = {0};
    char valueBuffer[35] = {0};
    for (int i = 0; i < MEM_BYTES / 8; i++) {
        turnToBaseN((uint64_t)(dir + i * 8), 16, buffer, BUFFER64_BITS);
        print(buffer);
        print(":  ");
        for (int j = 0; j < 8; j++) {
            char memValue[3];
            turnToBaseN(dump[i * 8 + j], 16, memValue, 3);
            strcpy(valueBuffer + j * 3, memValue);
            valueBuffer[26 + j] = (dump[i * 8 + j] >= 0x21 && dump[i * 8 + j]) ? dump[i * 8 + j] : '.';
        }
        for (int k = 2; k < 24; k += 3) {
            valueBuffer[k] = ' ';
        }
        valueBuffer[24] = '|';
        valueBuffer[25] = ' ';
        valueBuffer[34] = 0;
        print(valueBuffer);
        println(" ");
    }
}

void time(int args, char argv[][25]) {
    if (!checkArgs(args, 0)) return;

    dateInfo info;
    getTime(&info);
    char fecha[19] = {0};
    print("Local date at the moment is: ");
    dateToString(&info, fecha, '/', 18);
    print(fecha);
    println("");
}

void showApps(int args, char argv[][25]) {
    if (!checkArgs(args, 0)) return;

    int color = 0xffd1dc;//0xf03fcd;

    for (int i = 0; i < PROGRAMS; i++) {
        printc(commands[i].name, color);
        printc(": ", color);
        printcln(commands[i].description, 0xcfd7e6);
    }
}

void chess(int args, char argv[][25]) {
    chessHandler(args, argv);
}

void clear(int args, char argv[][25]) {
    if (!checkArgs(args, 0)) return;
    clearScreen();
}

void throwDivZero() {
    int a = 0;
    int b = 5;
    a = b / a;
}

void throwInvOpCode() {
    invalidOpCode();
}

void about() {
    printcln("Shernell 2.0 by Arce Julian, Lombardi Matias & Domingues Paula.", 0xcfd7e6);
}

int checkArgs(int args, int expected) {
    if (args != expected) {
        println("Invalid number of arguments");
        return 0;
    }
    return 1;
}

void ps(){
    getProcessesList();
}

void greet(){
    unsigned int pid = getPid();
    unsigned int start = getElapsedTicks();
    unsigned int second = 5;
    char pidString[20] = {0};
    itoaTruncate(pid, pidString, 20);
//    while(1) {
//        unsigned int ticks = getElapsedTicks();
//        if((ticks - start) / 18 >= second) {
//            print(pidString);
//            print(" says hello - Seconds: ");
//            char buff[20] = {0}; //Cuando printInt funcione, cambiarlo (para eso tienen que estar las syscalls del mm
//            itoaTruncate((ticks - start) / 18, buff, 20);
//            println(buff);
//            start = ticks;
//        }
//    }
    while(1) {
        unsigned int ticks = getElapsedTicks();
        if((ticks - start) >= 1) {
            print(pidString);
            print(" ");
            start = ticks;
        }
        println("a");
        _exit(1);

    }
}

void loop(){
    createProcess((uint64_t *)&greet, 0, 0, 0, 0);
}

void kill(int args, char argv[][25]){
    if (!checkArgs(args, 1)) return;
    int pid = string10ToInt(argv[1]);
    killProcess(pid);
}

void nice(int args, char argv[][25]){
    if (!checkArgs(args, 2)) return;
    int pid = string10ToInt(argv[1]);
    int priority = string10ToInt(argv[2]);
    changeProcessPriority(pid, priority);
}

void block(int args, char argv[][25]){
    if (!checkArgs(args, 1)) return;
    int pid = string10ToInt(argv[1]);
    changeProcessState(pid);
}

void mem(int args, char argv[][25]) {
    if (!checkArgs(args, 0)) return;
    char buffer[256] = {0};
    getMemInfo(buffer);
    println(buffer);
}

