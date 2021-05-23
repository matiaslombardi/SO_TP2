#define MAX_LEVELS 27 // 2^23/2^18 = 32
#define MIN_LEVELS 10 // 2^5 = 32
#define TOTAL_LEVELS (MAX_LEVELS-MIN_LEVELS)
#define TOTAL_SIZE (1<<MAX_LEVELS)
#define AUX_ARRAY_SPACE ((0x100000 / sizeof(Node))+1)
#define POW(value) (1<<value)
#define TRUE 1
#define FALSE 0
#define NULL (void *) 0

#include <stdint.h>
#include "../../Userland/SampleCodeModule/utilities/test_util.h"

typedef struct Node {
    int hasContent;
    void *base;
} Node;

typedef struct Level {
    int blocksAmount;
    Node *array;
} Level;

static Level buckets[TOTAL_LEVELS + 1];
static int memoryInitialized = 0;
void *base;
void *heapInit;
void *heapMax;
uint64_t totalMemory;
uint64_t freeMemory;

int blocksPerLevel(int level) {
    return POW(level); // 2^level
}

int sizeBlockAtLevel(int level) {
    return (TOTAL_SIZE / blocksPerLevel(level));
}

int idxAtLevel(uint64_t dir, int level) {
    return (dir / (blocksPerLevel(level + MIN_LEVELS)));
}

void createNodes() {
    void *auxBase = base;
    int maxLevel = MAX_LEVELS;
    int nodeSize = sizeof(Node);
    int nodeCount;
    for (int i = 0; maxLevel-i >= 0; i++) {
        nodeCount = blocksPerLevel(i);
        buckets[maxLevel - i].blocksAmount = nodeCount; //cantidad de bloques
        buckets[maxLevel - i].array = auxBase;
        auxBase += nodeSize *(nodeCount); //dejo seteado en donde comienza la proxima lista
    }
    buckets[TOTAL_LEVELS].array[0].base = heapInit;
    buckets[TOTAL_LEVELS].array[0].hasContent = FALSE;
}

void buddyInit(void *initAddress) {
    base = initAddress;
    if (base == NULL) {
        return;
    }
    heapInit = initAddress + AUX_ARRAY_SPACE * sizeof(Node);
    heapMax = base + 0x9FFFFF;
    createNodes();
    memoryInitialized = 1;
}
int isPowerOf2(int index){
    return (index != 0) && ((index & (index - 1)) == 0);
}
unsigned int logB2(unsigned int num) {
//    unsigned int ans = 0;
//    while (x >>= 1) ans++; /*function from stack overflow*/
//    return ans;
    int i = 0;
    int aux = num;
    if(num != 0){
        while ( aux > 0 ){
            i++;
            aux = aux >> 1;
        }
    }
    if ( !isPowerOf2(num)){
        i++;
    }
    return i-1;
}

int getLevel(int size) {
    int level = logB2(size) - MIN_LEVELS;
    return (level < 0) ? 0 : level;
}

/* * - Move to parent:         index = (index - 1) / 2;
 * - Move to left child:     index = index * 2 + 1;
 * - Move to right child:    index = index * 2 + 2;
 * - Move to sibling:        index = ((index - 1) ^ 1) + 1;*/
int getBuddy(int idx) {
    return (idx % 2 == 0) ? idx + 1 : idx - 1;
}

int getParent(int idx) {
    return (idx >> 1);
}

int isLevelAvailRec(int level, int idx) {
    if (level == TOTAL_LEVELS) { //si itere hasta el fondo. Caso base -> no tengo buddy
        if (buckets[level].array[idx].hasContent) {
            return FALSE;
        } else {
            buckets[level].array[idx].hasContent = TRUE;
            return TRUE;
        }
    }
    if (buckets[level].array[getBuddy(idx)].hasContent) { //chequeo a mi pana, ta ocupado?
        buckets[level].array[idx].hasContent = TRUE;
        return TRUE;
    } else if (buckets[level + 1].array[getParent(idx)].hasContent) { // mi padre esta ocupado?
        return FALSE;
    } else {
        if (isLevelAvailRec(level + 1, getParent(idx))) {
            buckets[level].array[idx].hasContent = TRUE;
            return TRUE;
        } else {
            return FALSE;
        }
    }
}

int isLevelAvailable(int level, int idx) {
    return isLevelAvailRec(level, idx);
}

void *buddyMalloc(int size) {
    // if (!memoryInitialized) initList((void*) 60000000); //esto saquemos dsp
    if (size <= 0 || size > (uint64_t) (heapMax-base))
        return NULL; //aca mas que total size seria delimitar hasta donde puede ir en direccion de memoria y fijarse que no se pase de ahi
    int level = getLevel(size);
    for (int i = 0; i < buckets[level].blocksAmount; i++) { //busco un bloque disponible en este nivel
        if (!buckets[level].array[i].hasContent && isLevelAvailable(level, i)) {
            //actualizar memoria utilizada para el contador
            return heapInit + i * blocksPerLevel(level + MIN_LEVELS); //direccion en la cual lo ubico.
            //dentro del array [level] en el bloque [i]
        }
    }
    return NULL;
}

int getIdxByLevel(uint64_t address, int level) {
    return address / blocksPerLevel(level + MIN_LEVELS);
}

int imLeft(int idx) {
    return idx % 2 == 0;
}

void buddyFree(void *ptr) {
    uint64_t address= (uint64_t)(ptr - heapInit);
    if (address < 0 || address > blocksPerLevel(MAX_LEVELS) || address % blocksPerLevel(MIN_LEVELS) != 0) { //si no es un bloque valido
        return;
    }
    int idx;
    for (int level = 0; level < TOTAL_LEVELS + 1; level++) {
        if ((address % blocksPerLevel(level + MIN_LEVELS)) != 0) { //si no esta alineado
            return;
        }

        idx = getIdxByLevel(address, level);
        if (level == MAX_LEVELS) {//keine buddy, estoy en el lvl max
            buckets[level].array[idx].hasContent = FALSE;
            return;
        }
        if (!buckets[level].array[idx].hasContent) { //veo si mi padre esta fragmentado
            if (buckets[level].array[getBuddy(idx)].hasContent) {
                return; //el buddy esta ocupado!
            }
        } else {
            buckets[level].array[idx].hasContent = FALSE;
            if (buckets[level].array[getBuddy(idx)].hasContent) { //buddy ocupado
                return;
            } else {
                if (!imLeft(idx)) { //ambos estan desocupados y soy derecho
                    address -= blocksPerLevel(level + MIN_LEVELS);
                }
            }
        }
    }
}