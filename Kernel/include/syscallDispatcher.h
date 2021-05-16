#include <stdint.h>
#include <videoDriver.h>
#include <keyboardDriver.h>
#include <timerDriver.h>
#include <library.h>
#include <time.h>
#include <scheduler.h>
#include <memDump.h>

#define MEM_BYTES 32

typedef int (*f)(uint64_t, ...);

int readHandler(uint64_t length, uint64_t toRead);

int writeHandler(uint64_t length, uint64_t toWrite, uint64_t row, uint64_t col, uint64_t color);

int drawHandler(uint64_t matrix, uint64_t row, uint64_t col, uint64_t rows, uint64_t columns);

int getTime(uint64_t myDate);

int memDumpHandler(uint64_t dir, uint64_t dump);

int infoRegHandler(uint64_t firstP);

int clearScreenHandler();

int screenHeightHandler();

int screenWidthHandler();

int psHandler();

int createProcessHandler(uint64_t entryPoint, uint64_t foreground);

int changeStateHandler(uint64_t pid, uint64_t state);

int changePriorityHandler(uint64_t pid, uint64_t priority);

int getPidHandler();

int getElapsedTicksHandler();
