// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <chessHandler.h>
#include <syscalls.h>
#include <standardIO.h>
#include <types.h>
#include <string.h>
#include <stdint.h>
#include <lib64.h>
#include <apps.h>
#include <phylo.h>
#include <tests.h>

#define MEM_BYTES 32
#define BUFFER64_BITS 20

char array[] = {121, 27, 3, 4, 5, 6, 7, 8, 9, 10};

programs commands[] = {{"about",       about,          "      Information about the O.S and authors.", 0},
                       {"help",        showApps,       "       Menu of the differents apps.", 0},
                       {"time",        time,           "       Displays the systems current time.", 0},
                       {"inforeg",     infoReg,        "    Displays the registers current state.", 0},
                       {"printmem",    printMem,       "   Prints on screen the first 32 bytes from a given position.", 0},
                       {"chess",       chess,          "      Starts a PVP chess match. Try chess help for aditional info.", 0},
                       {"clear",       clear,          "      Clears the current screen.", 0},
                       {"ps",          ps,             "         Displays a list of all the running processes with relevant data.", 0},
                       {"loop",        loop,           "       Prints a process ID given a determined number of seconds.", 0},
                       {"kill",        kill,           "       Kills a process given its ID.", 0},
                       {"nice",        nice,           "       Changes the given process priority.", 0},
                       {"block",       block,          "      Changes the process state given its ID.", 0},
                       {"mem",         mem,            "        Displays the current state of the memory.", 0},
                       {"sem",         sem,            "        Displays de current state of the semaphores", 0},
                       {"pipe",        pipe,           "       Displays de current state of the semaphores", 0},
                       {"cat",         catWrapper,     "        Prints from STDIN as received.", 1},
                       {"wc",          wcWrapper,      "         Retrieves the amount of lines from input." ,1},
                       {"filter",      filterWrapper,  "     Filters the vowels of the input.", 1},
                       {"phylos",      phylos,         "     Create a new philosophers table. Add with '+', delete with '-'. ", 0},
                       {"exceptionZ",  throwDivZero,   " Throws a divide by zero exception", 0},
                       {"exceptionOP", throwInvOpCode, "Throws an invalid Operation Code Exception", 0},
                       {"testMM",      testMM,         "     Run Memory Manager tests.", 0},
                       {"testPRIO",    testPRIO,       "   Run Processes Priority tests.", 0},
                       {"testPROC",    testPROC,       "   Run Processes creation, deletion and blocking tests.", 0},
                       {"testSYNC",    testSYNC,       "   Run semaphores sync tests.", 0},
                       {"testNoSYNC",  testNoSYNC,     "   Run without semaphores sync tests.", 0}
};

int checkArgs(int args, int expected);

int infoReg(int args, char argv[][25]) {
    if (!checkArgs(args, 0)){
        return - 1;
    }

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
    return 0;
}

int printMem(int args, char argv[][25]) {
    if (!checkArgs(args, 1)){
        return - 1;
    }

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

    return 0;
}

int time(int args, char argv[][25]) {
    if (!checkArgs(args, 0)){
        return -1;
    }

    dateInfo info;
    getTime(&info);
    char fecha[19] = {0};
    print("Local date at the moment is: ");
    dateToString(&info, fecha, '/', 18);
    print(fecha);
    println("");

    return 0;
}

int showApps(int args, char argv[][25]) {
    if (!checkArgs(args, 0)){
        return - 1;
    }

    int color = 0xffd1dc;//0xf03fcd;

    for (int i = 0; i < PROGRAMS; i++) {
        printc(commands[i].name, color);
        printc(": ", color);
        printcln(commands[i].description, 0xcfd7e6);
    }

    return 0;
}

int chess(int args, char argv[][25]) {
    chessHandler(args, argv);
    return 0;
}

int clear(int args, char argv[][25]) {
    if (!checkArgs(args, 0)){
        return - 1;
    }
    clearScreen();

    return 0;
}

int throwDivZero() {
    int a = 0;
    int b = 5;
    a = b / a;
    return 0;
}

int throwInvOpCode() {
    invalidOpCode();
    return 0;
}

int about() {
    printcln("Shernell 2.0 by Arce Julian, Lombardi Matias & Domingues Paula.", 0xcfd7e6);
    return 0;
}

int checkArgs(int args, int expected) {
    if (args != expected) {
        println("Invalid number of arguments");
        return 0;
    }
    return 1;
}

int ps(){
    getProcessesList();
    return 0;
}

int greet(){
    unsigned int pid = getPid();
    unsigned int start = getElapsedTicks();
    unsigned int seconds = 5;
    char pidString[20] = {0};
    itoaTruncate(pid, pidString, 20);
//    while(1) {
//        unsigned int ticks = getElapsedTicks();
//        if((ticks - start) / 18 >= seconds) {
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
    }
    return 0;
}

int loop(int args, char argv[][25]){
    int fdIn = 0;
    int fdOut = 1;
    if(args == 3) {
        fdIn = string10ToInt(argv[2]);
        fdOut = string10ToInt(argv[3]);
    }
    createProcess((uint64_t *)&greet, 0, fdIn, fdOut, 0, 0, 0);
    return 0;
}

int kill(int args, char argv[][25]){
    if (!checkArgs(args, 1)) {
        return -1;
    }

    int pid = string10ToInt(argv[1]);
    killProcess(pid);
    return 0;
}

int nice(int args, char argv[][25]){
    if (!checkArgs(args, 2)) {
        return -1;
    }

    int pid = string10ToInt(argv[1]);
    int priority = string10ToInt(argv[2]);
    changeProcessPriority(pid, priority);
    return 0;
}

int block(int args, char argv[][25]){
    if (!checkArgs(args, 1)) {
        return -1;
    }

    int pid = string10ToInt(argv[1]);
    changeProcessState(pid);
    return 0;
}

int mem(int args, char argv[][25]) {
    if (!checkArgs(args, 0)) {
        return -1;
    }

    char buffer[256] = {0};
    getMemInfo(buffer);
    println(buffer);
    return 0;
}

int sem(int args, char argv[][25]) {
    if (!checkArgs(args, 0)) {
        return -1;
    }

    char buffer[512] = {0};
    semInfo(buffer);
    println(buffer);
    return 0;
}

int pipe(int args, char argv[][25]) {
    if (!checkArgs(args, 0)) {
        return -1;
    }

    char buffer[512] = {0};
    pipeInfo(buffer);
    println(buffer);
    return 0;
}

int catWrapper(int args, char argv[][25]) { //recibe argv[0]fg,  argv[1]fdIN, argv[2]fdOut
    int fg = 1;
    int fdIn = 0;
    int fdOut = 1;
    if(args == 1) {
        fg = string10ToInt(argv[1]);
    }
    if(args == 3) {
        fg = string10ToInt(argv[1]);
        fdIn = string10ToInt(argv[2]);
        fdOut = string10ToInt(argv[3]);
    }

    int pid = createProcess((uint64_t *) &cat, fg, fdIn, fdOut, 0, 0, 0);
    return pid;
}

int cat(int args, char argv[][25]){
    int i = 0;
    char c, buffer[50] = {0};
    while((c=getChar())!= -1) {
        if (c != 0) {
            switch (c) {
                case '\b':
                    if (i > 0) {
                        buffer[--i] = 0;
                        removeChar();
                    }
                    break;
                case '\n':
                    println("");
                    buffer[i] = 0;
                    //println(buffer);
                    i = 0;
                    break;
                default:
                    buffer[i++] = c;
                    putChar(c);
            }
        }
    }
    _exit(0);
    return 0;
}

int wcWrapper(int args, char argv[][25]) { //recibe argv[0]fg,  argv[1]fdIN, argv[2]fdOut
    int fg = 1;
    int fdIn = 0;
    int fdOut = 1;
    if(args == 1) {
        fg = string10ToInt(argv[1]);
    }
    if(args == 3) {
        fg = string10ToInt(argv[1]);
        fdIn = string10ToInt(argv[2]);
        fdOut = string10ToInt(argv[3]);
    }

    int pid = createProcess((uint64_t *) &wc, fg, fdIn, fdOut, 0, 0, 0);
    return pid;
}

int wc(int args, char argv[][25]){
    int lines = 0;
    int c;
    while((c=getChar())!= -1) {
        putChar(c);
        if (c == '\n'){
            lines++;
        }
    }
    print("Total lines: ");
    printInt(lines); println("");
    _exit(0);
    return 0;
}

int filterWrapper(int args, char argv[][25]) { //recibe argv[0]fg,  argv[1]fdIN, argv[2]fdOut
    int fg = 1;
    int fdIn = 0;
    int fdOut = 1;
    if(args == 1) {
        fg = string10ToInt(argv[1]);
    }
    if(args == 3) {
        fg = string10ToInt(argv[1]);
        fdIn = string10ToInt(argv[2]);
        fdOut = string10ToInt(argv[3]);
    }

    int pid = createProcess((uint64_t *) &filter, fg, fdIn, fdOut, 0, 0, 0);
    return pid;
}

#define IS_VOWEL(c) (((c) == 'a') || ((c) == 'e') || ((c) == 'i') || ((c) == 'o') || ((c) == 'u') || ((c) == 'A') || ((c) == 'E') || ((c) == 'I') || ((c) == 'O') || ((c) == 'U'))
int filter(int args, char argv[][25]){
    int i = 0;
    char c, buffer[50] = {0};
    while((c=getChar()) != -1) {
        if (c != 0) {
            switch (c) {
                case '\b':
                    if (i > 0) {
                        buffer[--i] = 0;
                        removeChar();
                    }
                    break;
                case '\n':
                    println("");
                    buffer[i] = 0;
                    println(buffer);
                    i = 0;
                    break;
                default:
                    if(!IS_VOWEL(c)) buffer[i++] = c;
                    putChar(c);
            }
        }
    }
    _exit(0);
    return 0;
}

int phylos(int args, char argv[][25]){
    int fg = 1;
    int fdIn = 0;
    int fdOut = 1;
    if(args == 1) {
        fg = string10ToInt(argv[1]);
    }
    if(args == 3) {
        fg = string10ToInt(argv[1]);
        fdIn = string10ToInt(argv[2]);
        fdOut = string10ToInt(argv[3]);
    }
    int pid = createProcess((uint64_t *) &initPhylos, fg, fdIn, fdOut, 0, 0, 0);
    return pid;
}

int testMM(){
    createProcess((uint64_t *) &testMm, 0, 0, 1, 0, 0, 0);
    println("");
    return 0;
}

int testPRIO(){
    testPrio();
    println("");
    return 0;
}

int testPROC(){
    createProcess((uint64_t *)&testProcesses, 0, 0, 1, 0, 0, 0);
    println("");
    return 0;
}

int testSYNC(){
    testSync();
    println("");
    return 0;
}

int testNoSYNC(){
    testNoSync();
    println("");
    return 0;
}