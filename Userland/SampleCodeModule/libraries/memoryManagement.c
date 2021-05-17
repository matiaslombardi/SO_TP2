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
