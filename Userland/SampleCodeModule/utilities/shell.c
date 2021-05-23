#include <shell.h>
#include <string.h>
#include <standardIO.h>
#include <syscalls.h>
#include <stdint.h>
#include <memoryManagement.h>
#include <apps.h>

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

//    test_mm();
//    test_prio();
//    test_processes();
//    test_no_sync();
//    test_sync();

//    int pipeFd[2];
//    pipeOpen(pipeFd);
//    char buffer[5];
//    read(pipeFd[0], buffer, 1);

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

                int pid1;
                if (progAvail != -1) {
                    pid1 = createProcess((uint64_t *) commands[progAvail].f, 1, 0, fd[1], 0, 0, 0);
                }

                progAvail = hasProgram(tokens[1]);

                int pid2;
                if (progAvail != -1) {
                    pid2 = createProcess((uint64_t *) commands[progAvail].f, 0, fd[0], 1, 0, 0, 0);
                }

                waitPid(pid1);
                waitPid(pid2);

            } else {
                println("");
                char tokens[10][25] = {{0}}; //time param1
                int args = tokenizeArguments(content, tokens) - 1;// 1 es el nombre del programa
                int progAvail = hasProgram(tokens[0]);
                if (progAvail != -1) {
                    commands[progAvail].f(args, tokens);

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