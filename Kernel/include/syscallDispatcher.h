#include <stdint.h>
#include <videoDriver.h>
#include <keyboardDriver.h>
#include <timerDriver.h>
#include <library.h>
#include <time.h>
#include <scheduler.h>
#include <memDump.h>
#include <memoryManagement.h>
#include <semaphore.h>
#include <defs.h>
#include <pipes.h>

#define MEM_BYTES 32

typedef uint64_t (*syscall_t)(uint64_t rax, uint64_t rbx, uint64_t rcx, uint64_t rdx, uint64_t rsi, uint64_t rdi,
        uint64_t stackA);
//typedef uint64_t (*f)(uint64_t, ...);
//extern f syscalls[18];

uint64_t readHandler(uint64_t fd, uint64_t length, uint64_t toRead, uint64_t fourthP, uint64_t fifthP, uint64_t sixthP, uint64_t seventhP);

uint64_t writeHandler(uint64_t fd, uint64_t length, uint64_t toWrite, uint64_t row, uint64_t col, uint64_t color, uint64_t seventhP);

uint64_t drawHandler(uint64_t matrix, uint64_t row, uint64_t col, uint64_t rows, uint64_t columns, uint64_t sixthP, uint64_t seventhP);

uint64_t clearScreenHandler(uint64_t firstP, uint64_t secondP, uint64_t thirdP,
                            uint64_t fourthP, uint64_t fifthP, uint64_t sixthP, uint64_t seventhP);

uint64_t getElapsedTicksHandler(uint64_t firstP, uint64_t secondP, uint64_t thirdP,
                                uint64_t fourthP, uint64_t fifthP, uint64_t sixthP, uint64_t seventhP);

uint64_t exitHandler(uint64_t firstP, uint64_t secondP, uint64_t thirdP, uint64_t fourthP,uint64_t fifthP, uint64_t sixthP, uint64_t seventhP);

uint64_t infoRegHandler(uint64_t firstP, uint64_t secondP, uint64_t thirdP,
                        uint64_t fourthP, uint64_t fifthP, uint64_t sixthP, uint64_t seventhP);

uint64_t memDumpHandler(uint64_t dir, uint64_t dump, uint64_t thirdP,
                        uint64_t fourthP, uint64_t fifthP, uint64_t sixthP, uint64_t seventhP);

uint64_t getTime(uint64_t myDate, uint64_t secondP, uint64_t thirdP,
                 uint64_t fourthP, uint64_t fifthP, uint64_t sixthP, uint64_t seventhP);

uint64_t setAlarmHandler(uint64_t firstP, uint64_t secondP, uint64_t thirdP,
                         uint64_t fourthP, uint64_t fifthP, uint64_t sixthP, uint64_t seventhP);

uint64_t screenHeightHandler(uint64_t firstP, uint64_t secondP, uint64_t thirdP,
                             uint64_t fourthP, uint64_t fifthP, uint64_t sixthP, uint64_t seventhP);

uint64_t screenWidthHandler(uint64_t firstP, uint64_t secondP, uint64_t thirdP,
                            uint64_t fourthP, uint64_t fifthP, uint64_t sixthP, uint64_t seventhP);

uint64_t psHandler(uint64_t firstP, uint64_t secondP, uint64_t thirdP,
                   uint64_t fourthP, uint64_t fifthP, uint64_t sixthP, uint64_t seventhP);

uint64_t createProcessHandler(uint64_t entryPoint, uint64_t foreground, uint64_t thirdP,
                              uint64_t fourthP, uint64_t fifthP, uint64_t sixthP, uint64_t seventhP);

uint64_t changeStateHandler(uint64_t pid, uint64_t state, uint64_t thirdP,
                            uint64_t fourthP, uint64_t fifthP, uint64_t sixthP, uint64_t seventhP);

uint64_t changePriorityHandler(uint64_t pid, uint64_t priority, uint64_t thirdP,
                               uint64_t fourthP, uint64_t fifthP, uint64_t sixthP, uint64_t seventhP);

uint64_t getPidHandler(uint64_t firstP, uint64_t secondP, uint64_t thirdP,
                       uint64_t fourthP, uint64_t fifthP, uint64_t sixthP, uint64_t seventhP);

uint64_t getMemHandler(uint64_t size, uint64_t secondP, uint64_t thirdP,
                       uint64_t fourthP, uint64_t fifthP, uint64_t sixthP, uint64_t seventhP);

uint64_t freeMemHandler(uint64_t ptr, uint64_t secondP, uint64_t thirdP,
                        uint64_t fourthP, uint64_t fifthP, uint64_t sixthP, uint64_t seventhP);

uint64_t getMemInfoHandler(uint64_t toWrite, uint64_t secondP, uint64_t thirdP,
                           uint64_t fourthP, uint64_t fifthP, uint64_t sixthP, uint64_t seventhP);

uint64_t semOpenHandler(uint64_t semId, uint64_t initialValue, uint64_t thirdP,
                 uint64_t fourthP, uint64_t fifthP, uint64_t sixthP, uint64_t seventhP);

uint64_t semCloseHandler(uint64_t semId, uint64_t secondP, uint64_t thirdP,
                  uint64_t fourthP, uint64_t fifthP, uint64_t sixthP, uint64_t seventhP);

uint64_t semPostHandler(uint64_t semId, uint64_t secondP, uint64_t thirdP,
                 uint64_t fourthP, uint64_t fifthP, uint64_t sixthP, uint64_t seventhP);

uint64_t semWaitHandler(uint64_t semId, uint64_t secondP, uint64_t thirdP,
                    uint64_t fourthP, uint64_t fifthP, uint64_t sixthP, uint64_t seventhP);

uint64_t pipeOpenHandler(uint64_t fd, uint64_t secondP, uint64_t thirdP, uint64_t fourthP,
                         uint64_t fifthP, uint64_t sixthP, uint64_t seventhP);

uint64_t pipeCloseHandler(uint64_t fd, uint64_t secondP, uint64_t thirdP, uint64_t fourthP,
                          uint64_t fifthP, uint64_t sixthP, uint64_t seventhP);