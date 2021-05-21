#include <circularQueue.h>

/**
 * Definición de estructuras
 */
typedef struct Node {
    PCB * pcb;
    struct Node * next;
} Node;

typedef struct Queue {
    Node * first;
    Node * last;
    Node * iter;
    int iterSetted;
} Queue;


/**
 * Prototipo de Funciones auxiliares
 */
static void freeQueueRec(Node * first);

/**
 * Funciones principales
 */
QueueADT newQueue() {
    QueueADT queue;
    if((queue = mmMalloc(sizeof(Queue))) == NULL) {
        //Handler error
    }
    return queue;
}

void freeQueue(QueueADT queue) {
    if(queue == NULL) {
        return;
    }
    freeQueueRec(queue->first);
    mmFree(queue);
}

void freeQueueRec(Node * first) {
    if(first == NULL) {
        return;
    }
    freeQueueRec(first->next);
    mmFree(first);
}

void push(QueueADT queue, PCB *pcb){
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

PCB* pop(QueueADT queue) {
    if(queue->first == NULL) {
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

int isEmpty(QueueADT queue) {
    return queue->first == NULL;
}


PCB * findPCB(QueueADT queue, unsigned int pid) {
    Node * aux = queue->first;
    do {
        if(aux->pcb->pid == pid) {
            return aux->pcb;
        }
        aux = aux->next;
    } while(aux != queue->first);
    return NULL;
}

PCB * deleteNode(QueueADT queue, unsigned int pid) {
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

void toBegin(QueueADT queue) {
    queue->iterSetted = 0;
    queue->iter = queue->first;
}

int hasNext(QueueADT queue) {
    if(!queue->iterSetted) {
        queue->iterSetted = 1;
        return 1;
    }
    return queue->iter != queue->first;
}

PCB * next(QueueADT queue) {
    PCB * pcb = queue->iter->pcb;
    queue->iter = queue->iter->next;
    return pcb;
}