// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#ifndef FREE_LIST_MM

/*
 * Adapted from https://github.com/evanw/buddy-malloc/blob/master/buddy-malloc.c
 * Explained on https://www.wfbsoftware.de/a-buddy-system-memory-allocator-explained/
*/

#include <memoryManagement.h>
#include <string.h>
#include <library.h>

#define MAX_ALLOC_LOG2 25 // 2^23/2^18 = 32
#define MIN_ALLOC_LOG2 4 // 2^5 = 32
#define BUCKET_COUNT (MAX_ALLOC_LOG2-MIN_ALLOC_LOG2 + 1)
#define POW(x) (1<<(x))
#define TRUE 1
#define FALSE 0
#define HEADER_SIZE 8
#define MAX_ALLOC (1<<(MAX_ALLOC_LOG2)) //800000 bytes = 2^23
#define MIN_ALLOC (1<<(MIN_ALLOC_LOG2))


typedef struct buddyList {
    struct buddyList *next;
    struct buddyList *prev;
} buddyList;

/*
buckets[0] —> circular linked list of free blocks of whole memory space
buckets[1] —> circular linked list of free blocks of 1/2 memory space
buckets[2] —> circular linked list of free blocks of 1/4 memory space
buckets[3] —> circular linked list of free blocks of 1/8 memory space
 */
static buddyList buckets[BUCKET_COUNT];
static uint64_t isNodeSplit[(POW((BUCKET_COUNT - 1))) /
                            8]; //BUCKET_COUNT-1 because the leaves dont need to be managed. pow(n), the amount of in bits, that's why /8.
static uint64_t bucketLimit = BUCKET_COUNT - 1;
static uint64_t *base;
static uint64_t *maxHeap;
static uint64_t totalUsed;


#define HEAPSIZE (1024 * 1024 * 128)

static void initList(buddyList *list);
static void pushToList(buddyList *list, buddyList *entry);
static void removeFromList(buddyList *entry);
static buddyList *popFromList(buddyList *list);
static uint64_t *getPointerForNode(uint64_t idx, uint64_t bucket);
static uint64_t getNodeForPointer(uint64_t *ptr, uint64_t bucket);
static uint64_t bucketForRequest(uint64_t requested);
static void flipParentIsSplit(uint64_t index);
static int isParentSplit(uint64_t index);
static int lowerBucketLimit(uint64_t list);
static int checkMaxPointer(uint64_t *value);

void mmInit(void *initAddress) {
    base = initAddress;
    if (base == NULL) {
        return;
    }
    maxHeap = (base + HEAPSIZE);
    initList(&buckets[BUCKET_COUNT - 1]);
    pushToList(&buckets[BUCKET_COUNT - 1], (buddyList *) base);
    totalUsed = 0;
}

void *mmMalloc(uint64_t requested) {
    uint64_t originalBucket, bucket;
    if (requested + HEADER_SIZE > MAX_ALLOC) {
        return NULL;
    }
    bucket = bucketForRequest(requested + HEADER_SIZE);
    originalBucket = bucket;
    while (bucket + 1 != 0) {
        uint64_t size, bytesNeeded, i;
        uint64_t *ptr;
        if (!lowerBucketLimit(bucket)) {
            return NULL;
        }
        ptr = (uint64_t *) popFromList(&buckets[bucket]);
        if (!ptr) {
            if (bucket != bucketLimit || bucket == 0) {
                bucket--;
                continue;
            }
            if (!lowerBucketLimit(bucket - 1)) {
                return NULL;
            }
            ptr = (uint64_t *) popFromList(&buckets[bucket]);
        }
        size = (uint64_t) 1 << (MAX_ALLOC_LOG2 - bucket);
        bytesNeeded = bucket < originalBucket ? size / 2 + sizeof(buddyList) : size;
        if (checkMaxPointer(ptr +
                            bytesNeeded)) {
            pushToList(&buckets[bucket], (buddyList *) ptr);
            return NULL;
        }
        i = getNodeForPointer(ptr, bucket);
        if (i != 0) {
            flipParentIsSplit(i);
        }
        while (bucket < originalBucket) {
            i = i * 2 + 1;
            bucket++;
            flipParentIsSplit(i);
            pushToList(&buckets[bucket], (buddyList *) getPointerForNode(i + 1, bucket));
        }
        *(uint64_t *) ptr = requested;
        totalUsed += (POW(MAX_ALLOC_LOG2-bucket));
        return ptr + HEADER_SIZE;
    }
    return NULL;
}

void mmFree(void *ptr) {
    uint64_t bucket;
    uint64_t i;
    if (ptr == NULL || checkMaxPointer(ptr) || (uint64_t *) ptr < base/* || (uint64_t) ptr % HEADER_SIZE*/) {
        return;
    }
    ptr = (uint64_t *) ptr - HEADER_SIZE;
    bucket = bucketForRequest(*(uint64_t *) ptr + HEADER_SIZE);
    totalUsed -= (POW(MAX_ALLOC_LOG2-bucket));
    i = getNodeForPointer((uint64_t *) ptr, bucket);
    while (i != 0) {
        flipParentIsSplit(i);
        if (isParentSplit(i) || bucket == bucketLimit) {
            break;
        }
        removeFromList((buddyList *) getPointerForNode(((i - 1) ^ 1) + 1, bucket));
        i = (i - 1) / 2;
        bucket--;
    }
    pushToList(&buckets[bucket], (buddyList *) getPointerForNode(i, bucket));
}

static int checkMaxPointer(uint64_t *value) {
    if (value >= maxHeap || value < base) {
        return 1;
    }
    return 0;
}


static int isParentSplit(uint64_t index) {
    index = (index - 1) / 2;
    return (isNodeSplit[index / 8] >> (index % 8)) & 1;
}

static void flipParentIsSplit(uint64_t index) {
    index = (index - 1) / 2;
    isNodeSplit[index / 8] ^= 1 << (index % 8);
}


static uint64_t bucketForRequest(uint64_t requested) {
    uint64_t bucket = BUCKET_COUNT - 1;
    uint64_t size = MIN_ALLOC;
    while (size < requested) {
        bucket--;
        size *= 2;
    }
    return bucket;
}


static int lowerBucketLimit(uint64_t list) {
    while (list < bucketLimit) {
        uint64_t root = getNodeForPointer(base, bucketLimit);
        uint64_t *rightChild;
        if (!isParentSplit(root)) {
            removeFromList((buddyList *) base);
            initList(&buckets[--bucketLimit]);
            pushToList(&buckets[bucketLimit], (buddyList *) base);
            continue;
        }
        rightChild = getPointerForNode(root + 1, bucketLimit);
        if (checkMaxPointer(rightChild + sizeof(buddyList))) {
            return 0;
        }
        pushToList(&buckets[bucketLimit], (buddyList *) rightChild);
        initList(&buckets[--bucketLimit]);

        root = (root - 1) / 2;
        if (root != 0) {
            flipParentIsSplit(root);
        }
    }
    return 1;
}

static uint64_t *getPointerForNode(uint64_t idx, uint64_t bucket) {
    return base + ((idx - (POW(bucket)) + 1) << (MAX_ALLOC_LOG2 - bucket));
}

static uint64_t getNodeForPointer(uint64_t *ptr, uint64_t bucket) {
    return ((ptr - base) >> (MAX_ALLOC_LOG2 - bucket)) + (1 << bucket) - 1;
}

static void initList(buddyList *list) {
    list->prev = list;
    list->next = list;
}

static void pushToList(buddyList *list, buddyList *entry) {
    buddyList *prev = list->prev;
    entry->prev = prev;
    entry->next = list;
    prev->next = entry;
    list->prev = entry;
}

static void removeFromList(buddyList *entry) {
    buddyList *prev = entry->prev;
    buddyList *next = entry->next;
    prev->next = next;
    next->prev = prev;
}

static buddyList *popFromList(buddyList *list) {
    buddyList *back = list->prev;
    if (back == list) return NULL;
    removeFromList(back);
    return back;
}

void fillMemInfo(char *buffer) {
    uint64_t usedMemory = totalUsed;

    uint64_t total = MAX_ALLOC;
    uint64_t freeMemory = total - usedMemory;

    char aux[64] = {0};

    strcat(buffer, "Buddy Memory Management\n");

    strcat(buffer, "Total memory: ");
    itoaTruncate(total, aux, 64);
    strcat(buffer, aux);

    strcat(buffer, "\nUsed memory:  ");
    itoaTruncate(usedMemory, aux, 64);
    strcat(buffer, aux);

    strcat(buffer, "\nFree memory:  ");
    itoaTruncate(freeMemory, aux, 64);
    strcat(buffer, aux);

}

#endif