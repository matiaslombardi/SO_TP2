#include <standardIO.h>
#include <string.h>
#include <memoryManagement.h>
#include "test_util.h"
#include <apps.h>

#define MAX_BLOCKS 128
#define MAX_MEMORY (1024 * 1024 * 128 * 0.7) //Should be around 80% of memory managed by the MM
#define NULL (void *) 0

typedef struct MM_rq {
    void *address;
    uint32_t size;
} mm_rq;


void *memSet(void *destination, int32_t c, uint64_t length) {
    uint8_t chr = (uint8_t) c;
    char *dst = (char *) destination;

    while (length--)
        dst[length] = chr;

    return destination;
}

void test_mm() {
    mm_rq mm_rqs[MAX_BLOCKS];
    uint8_t rq;
    uint32_t total;

    while (1) {
//        printcln("Antes de asginar:", 0x04FF00);
//        mem(0, NULL);
        rq = 0;
        total = 0;

        // Request as many blocks as we can
        while (rq < MAX_BLOCKS && total < MAX_MEMORY) {
//        print(" disponible ");
            mm_rqs[rq].size = GetUniform(MAX_MEMORY - total - 1) + 1;
            mm_rqs[rq].address = mmMalloc(mm_rqs[rq].size); // TODO: Port this call as required
            if (mm_rqs[rq].address == NULL) {
                print("hasta ahora pedi: ");
                printInt(total);
                println("");
                print("quise asignar: ");
                printInt(mm_rqs[rq].size);
                println("");
                break;
            } else {
                total += mm_rqs[rq].size;
            }
//TODO: check if NULL
            rq++;
//            char b[15] = {0};
//            itoaTruncate(mm_rqs[rq].size, b, 10);
//            println(b);
        }
        print("asig: ");
        printInt(total);
        println("");
//        mem(0, NULL);

        // Set
        uint32_t i;
        for (i = 0; i < rq; i++)
            if (mm_rqs[i].address != NULL)
                memSet(mm_rqs[i].address, i, mm_rqs[i].size); // TODO: Port this call as required

        // Check
        for (i = 0; i < rq; i++)
            if (mm_rqs[i].address != NULL)
                if (!memcheck(mm_rqs[i].address, i, mm_rqs[i].size))
                    println("ERROR!"); // TODO: Port this call as required

        // Free
        uint64_t t = 0;
        for (i = 0; i < rq; i++) {
            if (mm_rqs[i].address != NULL) {
//                char b[10] = {0};
//                itoaTruncate(i, b, 10);
//                println(b);
                t += mm_rqs[i].size;

                mmFree(mm_rqs[i].address);  // TODO: Port this call as required
            }
        }

        print("lib: ");
        printInt(t);
        println("");
//        mem(0, NULL);
    }
}

/*
int main(){
  test_mm();
  return 0;
}
 */
