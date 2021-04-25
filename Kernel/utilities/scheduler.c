#include <scheduler.h>

#define STACK_SIZE 0x8000
#define REGISTER_AMOUNT 20
#define GENERAL_REGISTER_AMOUNT 15
#define START_STACK_SEGMENT 0x0
#define START_RFLAGS 0x202
#define START_CODE_SEGMENT 0x8

QueueADT processes;
unsigned int pidCounter = 1;
PCB* currentProcess = NULL;


static void fillPCB(PCB * pcb, unsigned int pid, uint64_t * base); //Falta foregroun y priority;


void initScheduler(){
    processes = newQueue();
}

unsigned int createProcess(uint64_t * entryPoint){
    uint64_t * base;
    if((base = mmMalloc(STACK_SIZE)) == NULL ){
        return 0;
    }
    base[STACK_SIZE - 1] = START_STACK_SEGMENT;
    base[STACK_SIZE - 2] = base + STACK_SIZE; //bp
    base[STACK_SIZE - 3] = START_RFLAGS;
    base[STACK_SIZE - 4] = START_CODE_SEGMENT;
    base[STACK_SIZE - 5] = entryPoint;      //rip
    for(int i = 0; i < GENERAL_REGISTER_AMOUNT; i++) {
        base[STACK_SIZE - i - 6] = GENERAL_REGISTER_AMOUNT - i; //De esta manera rax tendra un 1, si no es al reves
    }

    PCB * pcb;
    if((pcb= mmMalloc(sizeof(PCB))) == NULL) {
        return 0;
    }
    fillPCB(pcb, pidCounter, base);
    push(processes, pcb);
    return pidCounter++;
}

static void fillPCB(PCB * pcb, unsigned int pid, uint64_t * base) { //Falta foregroun y priority;
    pcb->pid = pid;
    pcb->state = READY;
    pcb->rsp = base + STACK_SIZE - REGISTER_AMOUNT;
    pcb->rbp = base + STACK_SIZE;
    pcb->priority = 0;
    pcb->foreground = 0;
}

uint64_t * switchProcesses(uint64_t * rsp){
    if(currentProcess != NULL) {
        currentProcess->rsp = rsp;
    }
    currentProcess = pop(processes);
    return currentProcess->rsp;
}

void switchStates(unsigned int processID){
    PCB *aux;
    if ((aux = findPCB(processes, processID) ) != NULL){
        aux->state = aux->state == BLOCKED? READY:BLOCKED;
    }
}

void endProcess(unsigned int processID){
    PCB *deleted;
    if((deleted = deleteNode(proccesses, processID))!= NULL){
        mmFree(deleted->rbp - STACK_SIZE);
        mmFree(deleted);
    }
}

void printProcesses(){
    toBegin(processes);
    while (hasNext(processes)){
        next(processes);
    }
}

void changePriorities(unsigned int processID, unsigned int newPriority){

}


