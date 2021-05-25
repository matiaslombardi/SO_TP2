// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <shell.h>
#include <string.h>
#include <standardIO.h>
#include <syscalls.h>
#include <stdint.h>
#include <memoryManagement.h>
#include <apps.h>
#include <phylo.h>

#define CHUNK 200

//-------------------------------------------
//                  SHERNELL
//-------------------------------------------

int hasProgram(char *input);

int tokenizeArguments(char *toToken, char tokens[10][25]);

char *initGreet = "user@TP2_SO:~$ ";

void initShell() {
//    char buffer[100] = {0};
//    getMemInfo(buffer);
//    println(buffer);
//
//    loop();
//
//    getMemInfo(buffer);
//    println(buffer);
//
//    char * s = mmMalloc(1);
//    getMemInfo(buffer);
//    println(buffer);
//
//    mmFree(s);

//
//    char * g = mmMalloc(256);
//    *buffer = 0;
//    getMemInfo(buffer);
//    println(buffer);

//    testMm();
//    testPrio();
//    testProcesses();
//    testNoSync();
//    testSync();

//    int pipeFd[2];
//    pipeOpen(pipeFd);
//    char buffer[5];
//    read(pipeFd[0], buffer, 1);

//    initPhylos();
//    phylo(0, 0);
    //int pipeFd[2];
    //pipeOpen(pipeFd);
    //pipeOpen(pipeFd);
    //pipeOpen(pipeFd);
    //pipeOpen(pipeFd);

    char c;
    int numPipes = 0;
    while (1) {
        printc(initGreet, 0x22ab00);
        char content[CHUNK] = {0};
        int contentLength = 0;
        while ((c = getChar()) != '\n') {
            if (c != 0) {
                switch (c) {
                    case '\b':
                        if (contentLength > 0) {
                            if(content[contentLength - 1] == '|'){
                                numPipes--;
                            }
                            content[--contentLength] = 0;
                            removeChar();
                        }
                        break;
                    default:
                        if(c == '|'){
                            numPipes++;
                        }
                        content[contentLength++] = c;
                        putChar(c);
                }
            }
        }
        content[contentLength] = 0;
        if (contentLength > 0) {
            if(numPipes > 0) {
                println("");
                char tokens[2][25] = {{0}};
                strtoks(content, '|', tokens);
                for(int i = 0; i < 2; i++) {
                    removeSpaces(tokens[i]);
                }
                int fd[2];
                pipeOpen(fd);
                int progAvail = hasProgram(tokens[0]);

                int pid1 = 0;
                if (progAvail != -1) {
                    char params[4][25] = {{0}};
                    strcpy(params[1], "1");
                    strcpy(params[2], "0");
                    char aux[4];
                    itoaTruncate(fd[1], aux, 4);
                    strcpy(params[3], aux);
                    pid1 = commands[progAvail].f(3, params);
                }

                progAvail = hasProgram(tokens[1]);

                int pid2 = 0;
                if (progAvail != -1) {
                    char params[4][25] = {{0}};
                    strcpy(params[1], "0");
                    char aux[4];
                    itoaTruncate(fd[0], aux, 4);
                    strcpy(params[2], aux);
                    strcpy(params[3], "1");
                    pid2 = commands[progAvail].f(3, params);
                }

                if(pid1 != 0) {
                    waitPid(pid1);
                }
                if(pid2 != 0) {
                    waitPid(pid2);
                }

            } else {
                println("");
                char tokens[10][25] = {{0}}; //time param1
                int args = tokenizeArguments(content, tokens) - 1;// 1 es el nombre del programa

                int bg = 0;
                if(tokens[0][0] == '&'){
                    bg = 1;
                    args = 1;
                    strcpy(tokens[0], tokens[0] + 1);
                }

                int progAvail = hasProgram(tokens[0]);
                if (progAvail != -1) {
                    if(bg == 1 && commands[progAvail].canBeBg ){
                        strcpy(tokens[1], "0");
                    }
                    int ans = commands[progAvail].f(args, tokens);
                    if(ans > 0){
                        waitPid(ans);
                    }
                } else {
                    print(content);
                    println(": command not found, try 'help' for help.");
                }
            }

        } else {
            println("");
        }

        contentLength = 0;
        numPipes = 0;
    }
}

int tokenizeArguments(char *toToken, char tokens[10][25]) {
    return strtoks(toToken, ' ', tokens);
}

int hasProgram(char *input) {
    for (int i = 0; i < PROGRAMS; i++) {
        if (strcmp(input, commands[i].name) == 0) {
            return i;
        }
    }
    return -1;
}