#include <memoryManagement.h>
#include <string.h>
#include <library.h>

typedef uint64_t align; // para alineamiento al limite mayor

union header {
    struct memNode {
        union header *next;
        uint64_t size;
    } memNode;
    align toAlign;
};

typedef union header Header;

static Header *firstFree = NULL;
//static Header heap[HEAPSIZE / sizeof(Header)];
static Header *heap;
static uint64_t totalHeap;

//void printHeapDir() {
//    printInt(heap);
//}

void mmInit(void *initAddress) {
    heap = (Header *) initAddress;
    firstFree = heap;
    firstFree->memNode.size = HEAPSIZE / sizeof(Header);
    firstFree->memNode.next = NULL;
    totalHeap = HEAPSIZE;
}

void *mmMalloc(uint64_t numBytesToAlloc) {
//    printcln("malloc", 0xFF0000);
    if (numBytesToAlloc == 0) {
        return NULL;
    }
    Header *current = firstFree;
    Header *previous = current;
    uint64_t unitsToAlloc = (numBytesToAlloc + sizeof(Header) - 1) / sizeof(Header) + 1; //equivalente a N unidades tamaño del Header

    for (current = firstFree; current != NULL; current = current->memNode.next) {
        if (current->memNode.size >= unitsToAlloc) { //si es lo suficientemente grande
            if (current->memNode.size == unitsToAlloc) { //si es exactamente el tamaño a allocar
                if (current == firstFree) {
                    firstFree = current->memNode.next;
                } else {
                    previous->memNode.next = current->memNode.next;
                }
            } else {
                current->memNode.size -= unitsToAlloc;
                current += current->memNode.size;
                current->memNode.size = unitsToAlloc;
            }
            return (void *) (current + 1);
        }
        previous = current;
    }
    return NULL;
}

void mmFree(void *ptr) {
//    printcln("free", 0x00FF00);

    Header *toFree;
    Header *current = firstFree;
    Header *previous;
    toFree = (Header *) ptr - 1; //Apunta al encabezado de un bloque

    //si toFree es null o menor a heap o mayor a heap + tamaño del heap
    if (toFree == NULL || toFree < heap || toFree >= heap + totalHeap) {
        return;
    }

    while (current != NULL && current < toFree) {
        previous = current;
        current = current->memNode.next;
    }

    if (current != NULL && toFree + toFree->memNode.size == current) {
        toFree->memNode.size += current->memNode.size;
        toFree->memNode.next = current->memNode.next;
    } else {
        toFree->memNode.next = current;
    }

    if (current == firstFree) {
        firstFree = toFree;
    } else {
        if (previous + previous->memNode.size == toFree) {
            previous->memNode.size += toFree->memNode.size;
            previous->memNode.next = toFree->memNode.next;
        } else {
            previous->memNode.next = toFree;
        }
    }
}

void fillMemInfo(char *buffer) {
    uint64_t freeMemory = 0;
    int freeBlocks = 0;
    Header *current = firstFree;

    while(current != NULL) {
        freeMemory += current->memNode.size;
        current = current->memNode.next;
        freeBlocks++;
    }

    freeMemory *= sizeof(Header);
    uint64_t total = totalHeap;
    uint64_t usedMemory = totalHeap - freeMemory;

    char aux[64] = {0};

    strcat(buffer, "Total memory: ");
    itoaTruncate(total, aux, 64);
    strcat(buffer, aux);

    strcat(buffer, "\nUsed memory:  ");
    itoaTruncate(usedMemory, aux, 64);
    strcat(buffer, aux);

    strcat(buffer, "\nFree memory:  ");
    itoaTruncate(freeMemory, aux, 64);
    strcat(buffer, aux);

    strcat(buffer, " in ");
    itoaTruncate(freeBlocks, aux, 64);
    strcat(buffer, aux);
    strcat(buffer, " blocks\n");
//    println("en kernel");
//    println(buffer);
//    println("-----");

}

/*

 Total memory: xxxxxxxxxxxxxxxx
 Free memory:  xxxxxxxxxxxxxxxx
 Used memory:  xxxxxxxxxxxxxxxx in xxxxxxxxxxxxxxxx blocks.

 * */