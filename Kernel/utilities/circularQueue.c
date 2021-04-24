#include "circularQueue.h"

/**
 * Definición de estructuras
 */
typedef struct Node {
    PCB pcb;
    struct Node * next;
} Node;

typedef struct Queue {
    Node * first;
    Node * last;
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

void push(QueueADT queue, PCB pcb){
    Node* aux;
    if( (aux = mmMalloc(sizeof(Node))) == NULL) {
        //Handler_error
    }
    aux->pcb = pcb;
    //aux->next = NULL;

    if(isEmpty(queue)) {
        queue->first = aux;
    }

    if(queue->last != NULL) {
        queue->last->next = aux;
    }
    queue->last = aux;
}

/*PCB pop(QueueADT queue) {
    if(queue->first == NULL) {
        //handler_error
    }
    Node* curr = queue->first;
    Node* prev = curr;
    while(curr != NULL && curr->pcb.state != READY) {
        prev = curr;
        curr = curr->next;
    }
    if(curr == NULL){
        //handler_error
    }
    PCB toReturn = curr->pcb;
    if(prev == queue->first){
        queue->first = queue->first->next;
    }
    else{
        prev->next = prev->next->next;
    }
    push(queue, toReturn);
    return toReturn;
}*/

PCB pop(QueueADT queue) {
    if(queue->first == NULL) {
        //handler_error
    }
    while(queue->first != NULL && queue->first->pcb.state != READY) {
        push(queue, queue->first->pcb);
        queue->first = queue->first->next;
    }
    if(queue->first == NULL) {
        //handler_error
    }
    PCB toReturn = queue->first->pcb;
    push(queue, toReturn);
    queue->first = queue->first->next;
    return toReturn;
}

int isEmpty(QueueADT queue) {
    return queue->first == NULL;
}

/*void printQueue(QueueADT queue) {
    Node * curr = queue->first;
    while(curr != NULL) {
        printf("Pid: %d - State: %s\n", (curr->pcb).pid, ((curr->pcb).state == 0) ? ("Ready") : ((curr->pcb).state == 1 ? "Blocked": "Killed"));
        curr = curr->next;
    }
}*/