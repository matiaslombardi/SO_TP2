// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <memoryManagement.h>

void * mmMalloc(uint64_t size) {
    return getMem(size);
}

void mmFree(void * ptr) {
    freeMem(ptr);
}

void getMemoryInfo(char * buffer) {
    getMemInfo(buffer);
}
