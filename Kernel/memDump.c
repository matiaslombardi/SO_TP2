#include <memDump.h>

#define MEM_BYTES 32
#define BUFFER64_BITS 20

void memDump(char *dir, char *dump) {
    for (int i = 0;
         i < MEM_BYTES; i++ ) {
        dump[i] = dir[i];
    }
}

void printMem(int args, char * dir) {

    //char *dir = (char *) stringToInt(argv[1]);
    char dump[MEM_BYTES];
    memDump(dir, dump);
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

