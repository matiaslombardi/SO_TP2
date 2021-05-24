#include <queue.h>

/**
 * Definición de estructuras
 */
typedef struct Elem {
    int pid;
    struct Elem * next;
} Elem;

typedef struct Queue {
    Elem * first;
    Elem * last;
    Elem * iter;
} Queue;

/**
 * Prototipo de Funciones auxiliares
 */
static void freeQueueRec(Elem * first);

/**
 * Funciones principales
 */
QueueADT newQueue(){
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

void freeQueueRec(Elem * first) {
    if(first == NULL) {
        return;
    }
    freeQueueRec(first->next);
    mmFree(first);
}

void enqueue(QueueADT queue, int pid) {
    Elem* aux;
    if( (aux = mmMalloc(sizeof(Elem))) == NULL) {
        return;
        //Handler_error
    }
    aux->pid = pid;
    aux->next = NULL;

    if(isEmpty(queue)) {
        queue->first = aux;
    }

    if(queue->last != NULL) {
        queue->last->next = aux;
    }
    queue->last = aux;
}

int dequeue(QueueADT queue) {
    if(isEmpty(queue)) {
        return 0;
    }
    int toReturn = queue->first->pid;

    Elem * aux = queue->first;
    if(queue->first == queue->last){
        queue->first = NULL;
        queue->last = NULL;
    } else {
        queue->first = queue->first->next;
    }
    mmFree(aux);
    return toReturn;
}

int isEmpty(QueueADT queue) {
    return queue->first == NULL;
}

void deleteWaiting(QueueADT queue, int pid) {
    if(isEmpty(queue)) {
        return;
    }

    Elem * current = queue->first;
    Elem * previous = NULL;

    while(current != NULL) {
        if(current->pid == pid) {
            if(previous != NULL) {
                previous->next = current->next;
            }
            if(current == queue->first) {
                queue->first = queue->first->next;
            }
            if(current == queue->last) {
                queue->last = previous;
            }
            mmFree(current);
            return;
        }
        previous = current;
        current = current->next;
    }
}

void toBegin(QueueADT queue) {
    queue->iter = queue->first;
}

int hasNext(QueueADT queue) {
    return queue->iter != NULL;
}

int next(QueueADT queue) {
    int pid = queue->iter->pid;
    queue->iter = queue->iter->next;
    return pid;
}

void printQueue(QueueADT queue) {

}