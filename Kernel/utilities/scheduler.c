// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <scheduler.h>

#include <semaphore.h>

#define STACK_SIZE 0x8000
#define REGISTER_AMOUNT 20
#define GENERAL_REGISTER_AMOUNT 15
#define START_STACK_SEGMENT 0x0
#define START_RFLAGS 0x202
#define START_CODE_SEGMENT 0x8
#define MAX_PRIORITY 5
#define MIN_PRIORITY 1

CircularQueueADT processes;
unsigned int pidCounter = 1;
PCB *currentProcess = NULL;


static void fillPCB(PCB *pcb, unsigned int pid, uint64_t *base, int foreground,
                    uint64_t fdIn, uint64_t fdOut); //Falta foregroun y priority;


void initScheduler() {
    processes = newCircularQueue();
}

unsigned int createProcess(uint64_t *entryPoint, int foreground, uint64_t fdIn, uint64_t fdOut,
                           uint64_t first, uint64_t second, uint64_t third) {
//    while(1) {
//        printInt(first); print(" ,"); printInt(second); print(" ,"); printInt(third); println("");
//    }
    uint64_t *base;
    if ((base = mmMalloc(STACK_SIZE)) == NULL) {
        return 0;
    }
    base[STACK_SIZE - 1] = START_STACK_SEGMENT;
    base[STACK_SIZE - 2] = (uint64_t)(base + STACK_SIZE); //rbp
    base[STACK_SIZE - 3] = START_RFLAGS;
    base[STACK_SIZE - 4] = START_CODE_SEGMENT;
    base[STACK_SIZE - 5] = (uint64_t) entryPoint;      //rip
    for (int i = 0; i < GENERAL_REGISTER_AMOUNT; i++) {
        base[STACK_SIZE - i - 6] = GENERAL_REGISTER_AMOUNT - i; //De esta manera rax tendra un 1, si no es al reves
    }


    base[STACK_SIZE - 11] = first; //rdi;
    base[STACK_SIZE - 12] = second; //rsi;
    base[STACK_SIZE - 9] = third; //rdx;

//    printInt(*(base + STACK_SIZE - 17)); println("");

    PCB *pcb;
    if ((pcb = mmMalloc(sizeof(PCB))) == NULL) {
        return 0;
    }
    fillPCB(pcb, pidCounter, base, foreground, fdIn, fdOut);
    circularEnqueue(processes, pcb);
    return pidCounter++;
}

static void fillPCB(PCB *pcb, unsigned int pid, uint64_t *base, int foreground,
                    uint64_t fdIn, uint64_t fdOut) { //Falta foreground y priority;
    pcb->pid = pid;
    pcb->state = READY;
    pcb->rsp = base + STACK_SIZE - REGISTER_AMOUNT;
    pcb->rbp = base + STACK_SIZE;
    pcb->priority = 1;
    pcb->tickets = 1;
    pcb->foreground = foreground;
    pcb->fdIn = fdIn;
    pcb->fdOut = fdOut;
    pcb->waitingPid = 0;
}

uint64_t *switchProcesses(uint64_t *rsp) {
    if (currentProcess != NULL) {
        currentProcess->rsp = rsp;

        if (currentProcess->state == READY && currentProcess->tickets > 0) {
            currentProcess->tickets--;
            return currentProcess->rsp;
        }

        currentProcess->tickets = currentProcess->priority;
    }

    currentProcess = circularDequeue(processes);
    currentProcess->tickets--;
//    print("scheduler: "); printInt(currentProcess->pid); println("");
    if(currentProcess->pid == 2) {
//        char buffer[512] = {0};
//        fillSemInfo(buffer);
//        print(buffer);
//        printProcesses();
    }
    return currentProcess->rsp;
}

void switchStates(unsigned int pid) {
    PCB *aux;
    if ((aux = findPCB(processes, pid)) != NULL) {
        aux->state = aux->state == BLOCKED ? READY : BLOCKED;
    }
}

void sleep(unsigned int pid) {
    PCB *aux;
    if ((aux = findPCB(processes, pid)) != NULL) {
        aux->state = BLOCKED;
    }
    _forceInt();
}

void wakeup(unsigned int pid) {
    PCB *aux;
    if ((aux = findPCB(processes, pid)) != NULL) {
        aux->state = READY;
    }
    _forceInt();
}

void endProcess(unsigned int pid) {
    PCB *deleted;

    if ((deleted = deleteNode(processes, pid)) != NULL) {
        removeWaitingPid(deleted->pid);
        pipeClose(deleted->fdIn);
        pipeClose(deleted->fdOut);
        mmFree(deleted->rbp - STACK_SIZE);
        mmFree(deleted);
    }
}

void printProcesses() {
    circularToBegin(processes);
    char toPrint[20] = {0};
    println("PID         State    Prior    RSP                      RBP                      FG    Name");//Falta imprimir state
    while (circularHasNext(processes)) {
        PCB *aux = circularNext(processes);
        //PID
        printInt(aux->pid);
        print("    ");
        for (int i = 0; i < 8 - numlen(aux->pid); i++) print(" ");
        //State
        print(aux->state == READY ? "Ready" : aux->state == BLOCKED ? "Block" : "Kill ");
        print("    ");
        //Priority
        printInt(aux->priority);
        print("        ");
        //RSP
        turnToBaseN((uint64_t) aux->rsp, 16, toPrint, 20);//uint64_t value, int base, char *buffer, int bufferLength
        print("0x");
        print(toPrint);
        print("    ");
        //RBP
        print("0x");
        turnToBaseN((uint64_t) aux->rbp, 16, toPrint, 20);//uint64_t value, int base, char *buffer, int bufferLength
        print(toPrint);
        print("    ");
        //FG
        print(aux->foreground == 0 ? "BG" : "FG");
        print("    ");
        //Name
        println(""); //Este es para el nombre del programa.
    }
}

void changePriorities(unsigned int pid, unsigned int newPriority) {
    PCB *aux;
    if (newPriority < MIN_PRIORITY || newPriority > MAX_PRIORITY) {
        return;
    }
    if ((aux = findPCB(processes, pid)) != NULL) {
        aux->priority = newPriority;
        aux->tickets = newPriority;
    }
}

unsigned int getPid() {
    return currentProcess->pid;
}

unsigned int getFg() {
    return currentProcess->foreground;
}

unsigned int getFdIn() {
    return currentProcess->fdIn;
}

unsigned int getFdOut() {
    return currentProcess->fdOut;
}

int addWaitingPid(unsigned int pid) {
    PCB *aux;
    if ((aux = findPCB(processes, pid)) != NULL) {
        aux->waitingPid = currentProcess->pid;
        sleep(currentProcess->pid);
    }
    return 0;
}

void exit() {
    int waiting = currentProcess->waitingPid;
    if (waiting != 0) {
        wakeup(waiting);
    }

    if(getFdOut() != STDOUT) {
        pipeClose(getFdOut());
    }
    if(getFdIn() != STDIN) {
        pipeClose(getFdIn());
    }

    endProcess(getPid());
    currentProcess = NULL;

    _forceInt();
}