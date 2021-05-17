#include <memoryManagement.h>
#include <string.h>
#include <library.h>

typedef uint64_t align; // para alineamiento al limite mayor 

union header{
    struct memNode{
        union header * next;
        uint64_t size;
    } memNode;
    align toAlign; 
};

typedef union header Header;

static Header base;
static Header *freep = NULL;
//static Header heap[HEAPSIZE / sizeof(Header)];
static Header * heap = (void *) 0x600000;


/* Estas funciones no van al .h */
static Header * morecore(uint64_t nu);

void printHeapDir() {
    printInt(heap);
}

void * mmMalloc(uint64_t numBytesToAlloc){
    Header *current, *previous;
    uint64_t unitsToAlloc;//nunits

    unitsToAlloc = (numBytesToAlloc + sizeof(Header) - 1)/sizeof(Header) + 1;
    if ((previous = freep) == NULL){
        base.memNode.next = freep = previous = &base; //crea una lista circular
        base.memNode.size = 0;
    }

    for (current = previous->memNode.next ; ;current = current->memNode.next){
        if (current->memNode.size >= unitsToAlloc){ //si es lo suficientemente grande
            if (current->memNode.size == unitsToAlloc){ //si es exactamente el tamaño a allocar
                previous->memNode.next = current->memNode.next;
            }else{
                current->memNode.size -= unitsToAlloc;
                current += current->memNode.size;
                current->memNode.size = unitsToAlloc;
            }
            freep = previous;
            return (void*) (current+1);
        }
        if (current == freep){
            if ((current = morecore(unitsToAlloc)) == NULL){ //dio vuelta la lista libre
                return NULL;
            }
        }
        previous = current;
    }
}

static Header * morecore(uint64_t unitsToAlloc) {
    static Header *up;
	
	if ( up ) {
		return 0;
    }
	up = heap;

	up->memNode.size = HEAPSIZE / sizeof(Header);
	mmFree(up + 1);
	return freep;
}


void mmFree(void * ptr) {
    Header *toFree;
    Header *p;
    toFree = (Header *) ptr - 1; //Apunta al encabezador de un bloque
    
    for(p = freep; !(toFree > p && toFree < p->memNode.next); p = p->memNode.next) {
        if(p >= p->memNode.next && (toFree > p || toFree < p->memNode.next)) {
            break; //Libera bloque al inicio o al final
        }
    }

    if(toFree + toFree->memNode.size == p->memNode.next) { //Une al nbr superior
        toFree->memNode.size += p->memNode.next->memNode.size;
        toFree->memNode.next = p->memNode.next->memNode.next;
    } else {
        toFree->memNode.next = p->memNode.next;
    }

    if(p + p->memNode.size == toFree) { //Une al nbr inferior
        p->memNode.size += toFree->memNode.size;
        p->memNode.next = toFree->memNode.next;
    } else {
        p->memNode.next = toFree;
    }

    freep = p;
}

void fillMemInfo(char * buffer) {
    uint64_t memory = 0;
    Header * current = heap;

    do {
        memory += current->memNode.size;
        current = current->memNode.next;
    } while(current != heap);

    memory *= sizeof(Header);
    itoaTruncate(memory, buffer, 64);
}