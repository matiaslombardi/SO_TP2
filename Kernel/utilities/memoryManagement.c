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

//static Header base;
static Header *firstFree = NULL;
//static Header heap[HEAPSIZE / sizeof(Header)];
static Header *heap;
static uint64_t totalHeap;


/* Estas funciones no van al .h */
//static Header *morecore(uint64_t nu);

void printHeapDir() {
    printInt(heap);
}

void mmInit(void *initAddress) {
    heap = (Header *) initAddress;
    firstFree = heap;
    firstFree->memNode.size = HEAPSIZE / sizeof(Header);
    firstFree->memNode.next = NULL;
    totalHeap = HEAPSIZE;
}

void *mmMalloc(uint64_t numBytesToAlloc) {
    if (numBytesToAlloc == 0) {
        return NULL;
    }
    Header *current = firstFree;
    Header *previous = current;
    uint64_t unitsToAlloc =
            (numBytesToAlloc + sizeof(Header) - 1) / sizeof(Header) + 1; //equivalente a N unidades tamaño del Header
//    if ((previous = freep) == NULL){
//        base.memNode.next = freep = previous = &base; //crea una lista circular
//        base.memNode.size = 0;
//    }

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
//            firstFree = previous;

            return (void *) (current + 1);
        }
//        if (current == firstFree) { //dio vuelta la lista libre
//            return NULL;
//        }
        previous = current;
    }
    return NULL;
}

//static Header *morecore(uint64_t unitsToAlloc) {
//    static Header *up;
//
//    if (up) {
//        return 0;
//    }
//    up = heap;
//
//    up->memNode.size = HEAPSIZE / sizeof(Header);
//    mmFree(up + 1);
//    return freep;
//}


void mmFree(void *ptr) {
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
//    for (current = firstFree;
//         current != NULL && !(current < toFree && current->memNode.next > toFree); current = current->memNode.next) {
//        if (current->memNode.next == NULL && (toFree > current || toFree < current->memNode.next)) {
//            break;
//        }
//        if (toFree < current) {
//            break;
//        }
//    }

//    if (current != NULL) {
//        if (toFree + toFree->memNode.size == current) { //Une al nbr superior
//            toFree->memNode.size += current->memNode.size;
//            toFree->memNode.next = current->memNode.next;
//        } else if(toFree < current->memNode.next){
//            toFree->memNode.next = current;
//        }
//
//        if (current + current->memNode.size == toFree) { //Une al nbr inferior
//            current->memNode.size += toFree->memNode.size;
//            current->memNode.next = toFree->memNode.next;
//        } else if(toFree > current->memNode.next) {
//            current->memNode.next = toFree;
//        }
//
//        firstFree = toFree < firstFree ? toFree:firstFree;
//    } else {
//        toFree->memNode.next = NULL;
//        firstFree = toFree;
//    }


//    firstFree = current < toFree ? current : toFree;

/*
void mmFree(void *ap)
{
    Header *curr, *prev, *aux;
    aux = (Header *)(ap);
    aux--;

    if (aux == NULL || aux < (Header *)heap || aux >= (Header *)heap + totalHeap)
    {
        return;
    }

    curr = firstFree;
    while (curr != NULL && curr < aux)
    { // Buscamos el anterior libre.
        prev = curr;
        curr = curr->memNode.next;
    }
    if (curr != NULL && (aux + aux->memNode.size == curr))
    { // Vemos de unir con el siguiente ptr.
        aux->memNode.size += curr->memNode.size;
        aux->memNode.next = curr->memNode.next;
    }
    else // Si no me puedo unir hago simplemente un cambio de puntero.
        aux->memNode.next = curr;

    if (curr == firstFree)
    { // Con esto veo si estoy antes que el primero libre.
        firstFree = aux;
    }
    else
    {
        if (prev + prev->memNode.size == aux)
        { // Uno los bloques si tengo uno libre atrás.
            prev->memNode.size += aux->memNode.size;
            prev->memNode.next = aux->memNode.next;
        }
        else
        {
            prev->memNode.next = aux; // Si no hago que el de atrás me apunte.
        }
    }
}*/

void fillMemInfo(char *buffer) {
    uint64_t memory = 0;
    Header *current = heap;

    do {
        memory += current->memNode.size;
        current = current->memNode.next;
    } while (current != heap);

    memory *= sizeof(Header);
    itoaTruncate(memory, buffer, 64);
}