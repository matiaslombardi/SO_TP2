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
} Queue;


/**
 * Prototipo de Funciones auxiliares
 */
static void freeQueueRec(Node * first);
static Node * deleteNodeRec(Node * first, unsigned int pid, PCB * pcb);

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
    aux->pcb = pcb;
    aux->next = NULL;

    if(isEmpty(queue)) {
        queue->first = aux;
    }

    if(queue->last != NULL) {
        queue->last->next = aux;
    }
    queue->last = aux;
}

PCB* pop(QueueADT queue) {
    if(queue->first == NULL) {
        return NULL;
    }
    while(queue->first != NULL && queue->first->pcb->state != READY) {
        push(queue, queue->first->pcb);
        Node * aux = queue->first;
        queue->first = queue->first->next;
        mmFree(aux);
    }
    if(queue->first == NULL) {
        return NULL;
    }
    PCB * toReturn = queue->first->pcb;
    push(queue, toReturn);
    Node * aux = queue->first;
    queue->first = queue->first->next;
    mmFree(aux);
    return toReturn;
}

int isEmpty(QueueADT queue) {
    return queue->first == NULL;
}


PCB * findPCB(QueueADT queue, unsigned int pid) {
    Node * aux = queue->first;
    while(aux != NULL) {
        if(aux->pcb->pid == pid) {
            return aux->pcb;
        }
        aux = aux->next;
    }
    return NULL;
}

PCB * deleteNode(QueueADT queue, unsigned int pid) {
    PCB * pcb = NULL;
    queue->first = deleteNodeRec(queue->first, pid, pcb);
    return pcb;
}

static Node * deleteNodeRec(Node * first, unsigned int pid, PCB * pcb) {
    if(first == NULL) {
        return first;
    }

    if(first->pcb->pid == pid) {
        Node * aux = first->next;
        pcb = first->pcb;
        mmFree(first);
        return aux;
    }
    first->next = deleteNodeRec(first->next, pid, pcb);
    return first;
}

void toBegin(QueueADT queue) {
    queue->iter = queue->first;
}

int hasNext(QueueADT queue) {
    return queue->iter != NULL;
}

PCB * next(QueueADT queue) {
    PCB * pcb = queue->iter->pcb;
    queue->iter = queue->iter->next;
    return pcb;
}