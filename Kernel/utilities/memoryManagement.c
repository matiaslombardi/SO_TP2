#include <memoryManagement.h>

/* Estas funciones van al .h */
//void * mmMalloc(size_t size); //Cambiar int por nuestro equivalente a size_t
//void mmFree(void * ptr);

typedef uint64_t align; // para alineamiento al limite mayor 

union header{
    struct memNode{
        union header *ptr;
        uint64_t size;
    } memNode;
    align toAlign; 
};

typedef union header Header;

static Header base;
static Header *freep = NULL;
static Header heap[HEAPSIZE / sizeof(Header)];

/* Estas funciones no van al .h */
static Header * morecore(uint64_t nu);

void * mmMalloc(uint64_t numBytesToAlloc){
    Header *current,*previous;
    uint64_t unitsToAlloc;//nunits

    unitsToAlloc = (numBytesToAlloc + sizeof(Header) - 1)/sizeof(Header) + 1;
    if ((previous = freep) == NULL){
        base.memNode.ptr = freep = previous = &base; //crea una lista circular
        base.memNode.size = 0;
    }
    for (current = previous->memNode.ptr ; ;current = current->memNode.ptr){
        if (current->memNode.size >= unitsToAlloc){ //si es lo suficientemente grande
            if (current->memNode.size == unitsToAlloc){ //si es exactamente el tamanio a allocar
                previous->memNode.ptr = current->memNode.ptr;
            }else{
                current->memNode.size -= unitsToAlloc;
                current += current->memNode.size;
                current->memNode.size = unitsToAlloc;
            }
            freep = previous;
            return (void*) (current+1);
        }
        if (current == freep){
            if ((current = morecore(unitsToAlloc))==NULL){ //dio vuelta la lista libre
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
    Header *header;
    Header *p;
    header = (Header *) ptr - 1; //Apunta al encabezador de un bloque
    
    for(p = freep; !(header > p && header < p->memNode.ptr); p = p->memNode.ptr) {
        if(p >= p->memNode.ptr && (header > p || header < p->memNode.ptr)) {
            break; //Libera bloque al inicio o al final
        }
    }

    if(header + header->memNode.size == p->memNode.ptr) { //Une al nbr superior
        header->memNode.size += p->memNode.ptr->memNode.size;
        header->memNode.ptr = p->memNode.ptr->memNode.ptr;
    } else {
        header->memNode.ptr = p->memNode.ptr;
    }

    if(p + p->memNode.size == header) { //Une al nbr inferior
        p->memNode.size += header->memNode.size;
        p->memNode.ptr = header->memNode.ptr;
    } else {
        p->memNode.ptr = header;
    }
    freep = p;
}