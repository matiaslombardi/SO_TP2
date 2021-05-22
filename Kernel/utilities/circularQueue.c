#include <circularQueue.h>

/**
 * Definición de estructuras
 */
typedef struct Node {
    PCB * pcb;
    struct Node * next;
} Node;

typedef struct CircularQueue {
    Node * first;
    Node * last;
    Node * iter;
    int iterSetted;
} CircularQueue;


/**
 * Prototipo de Funciones auxiliares
 */
static void freeCircularQueueRec(Node * first);

/**
 * Funciones principales
 */
CircularQueueADT newCircularQueue() {
    CircularQueueADT queue;
    if((queue = mmMalloc(sizeof(CircularQueue))) == NULL) {
        //Handler error
    }
    return queue;
}

void freeCircularQueue(CircularQueueADT queue) {
    if(queue == NULL) {
        return;
    }
    freeCircularQueueRec(queue->first);
    mmFree(queue);
}

static void freeCircularQueueRec(Node * first) {
    if(first == NULL) {
        return;
    }
    freeCircularQueueRec(first->next);
    mmFree(first);
}

void circularEnqueue(CircularQueueADT queue, PCB *pcb){
    Node* aux;
    if( (aux = mmMalloc(sizeof(Node))) == NULL) {
        return;
        //Handler_error
    }

    if(isEmpty(queue)) {
        queue->first = aux;
        queue->first->next = aux;
    }
    aux->pcb = pcb;
    aux->next = queue->first;

    if(queue->last != NULL) {
        queue->last->next = aux;
    }
    queue->last = aux;
    queue->last->next = queue->first;
}

PCB* circularDequeue(CircularQueueADT queue) {
    if(isEmptyCircular(queue)) {
        return NULL;
    }
    while(queue->first->pcb->state != READY) {
        queue->last = queue->first;
        queue->first = queue->first->next;
    }

    PCB * toReturn = queue->first->pcb;
    queue->last = queue->first;
    queue->first = queue->first->next;
    return toReturn;
}

int isEmptyCircular(CircularQueueADT queue) {
    return queue->first == NULL;
}


PCB * findPCB(CircularQueueADT queue, unsigned int pid) {
    Node * aux = queue->first;
    do {
        if(aux->pcb->pid == pid) {
            return aux->pcb;
        }
        aux = aux->next;
    } while(aux != queue->first);
    return NULL;
}

PCB * deleteNode(CircularQueueADT queue, unsigned int pid) {
    if(isEmpty(queue)) {
        return NULL;
    }
    PCB * pcb = NULL;
    Node * current = queue->first;
    Node * previous = queue->last;
    do {
        if(current->pcb->pid == pid) {
            pcb = current->pcb;
            if(current == queue->first) {
                queue->first = queue->first->next;
//                queue->last->next = queue->first;
            }
            if(current == queue->last) {
                queue->last = previous;
//                queue->last->next = queue->first;
            }
            previous->next = current->next;
            mmFree(current);
            return pcb;
        }
        previous = current;
        current = current->next;
    } while(current != queue->first);

    return NULL;
}

void circularToBegin(CircularQueueADT queue) {
    queue->iterSetted = 0;
    queue->iter = queue->first;
}

int circularHasNext(CircularQueueADT queue) {
    if(!queue->iterSetted) {
        queue->iterSetted = 1;
        return 1;
    }
    return queue->iter != queue->first;
}

PCB * circularNext(CircularQueueADT queue) {
    PCB * pcb = queue->iter->pcb;
    queue->iter = queue->iter->next;
    return pcb;
}