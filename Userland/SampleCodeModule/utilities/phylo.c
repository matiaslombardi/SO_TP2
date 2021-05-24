#include <phylo.h>
#include <syscalls.h>
#include <string.h>
#include <standardIO.h>

#define MAX_PHYLOS 8
#define MIN_PHYLOS 2
#define CREATING_SEM "createPhylo"
#define FORK_SEM "forkSem"
#define PRINTER_SEM "printerSem"
#define STATE_CHANGING_SEM "stateSem"

static int total = MIN_PHYLOS;
static int phyloIdCount = 0;

static uint64_t phylos[MAX_PHYLOS] = {0};
static int phylosPid[MAX_PHYLOS] = {0};
static uint64_t state[MAX_PHYLOS] = {0};
static char sems[MAX_PHYLOS][10] = {{0}};

//int phyloAdded = 0;

void phylo(int id);

void printPhylos();

static void createPhylo();

static void addPhylo();

static void removePhylo();

void initPhylos() {
    semOpen(CREATING_SEM, 1);
    semOpen(STATE_CHANGING_SEM, total);

    semWait(CREATING_SEM);

    for (int i = 0; i < total; i++) {
        createPhylo();
    }

    print("Welcome to Dining Philosophers. Press 'a' for add, press 'r' for remove, press 'q' for quit.\n");

    for (int i = 0; i < total; i++) {
        semPost(CREATING_SEM);
    }

    char c;
    while ((c = getChar()) != 'q') {
        if (c != 0) {
            switch (c) {
                case 'a':
                    if (total <= MAX_PHYLOS) {
                        addPhylo();
                    }
                    break;
                case 'r':
                    if (total > MIN_PHYLOS) {
                        removePhylo();
                    }
                    break;
            }
        }
    }

    //TODO manejar memoria
    _exit(0);
}

static void createPhylo() {
    char semName[20] = {0};
    char semNameId[5] = {0};
    itoaTruncate(phyloIdCount, semNameId, 5);

    strcat(semName, FORK_SEM);
    strcat(semName, semNameId);

    semOpen(semName, 1);
    strcpy(sems[phyloIdCount], semName);

    phylos[phyloIdCount] = phyloIdCount;
    phylosPid[phyloIdCount] = createProcess((uint64_t * ) & phylo, 0, 0, 1, phyloIdCount, 0, 0);
    phyloIdCount++;
}

static void addPhylo() {
    for (int i = 0; i < total; i++) {
        semWait(STATE_CHANGING_SEM);
    }

    createPhylo();

    printc("Phylo added\n", 0xff0000);


    total++;

    for (int i = 0; i < total; i++) {
        semPost(STATE_CHANGING_SEM);
    }

    semPost(CREATING_SEM);

}

static void removePhylo() {
    for (int i = 0; i < total; i++) {
        semWait(STATE_CHANGING_SEM);
    }
    total--;
    semClose(sems[total]);
    killProcess(phylosPid[--phyloIdCount]);
    printc("Phylo removed\n", 0x0022ff);

    for (int i = 0; i < total; i++) {
        semPost(STATE_CHANGING_SEM);
    }
}

void phylo(int id) {
//    semOpen(CREATING_SEM, 1);
//    semOpen(PRINTER_SEM, 1);
//
//    semOpen(STATE_CHANGING_SEM, total);

    semWait(CREATING_SEM);

    char *first;
    char *second;

    while (1) {
        semWait(STATE_CHANGING_SEM);

        if (id % 2 == 0) {
            first = sems[(id - 1 + phyloIdCount) % phyloIdCount];
            second = sems[id];
        } else {
            first = sems[id];
            second = sems[id - 1];
        }

        for (uint64_t i = 0; i < 50000000; i++);

        semWait(first);
        semWait(second);

        state[id] = 1;

        for (int i = 0; i < total; i++) {
            if (state[i] == 1) printc("E", 0x00ff08);
            else printc(".", 0x00ff08);
        }
        println("");

        for (uint64_t i = 0; i < 50000000; i++);

        state[id] = 0;

        semPost(second);
        semPost(first);

        semPost(STATE_CHANGING_SEM);
    }
}

//void printPhylos() {
//    semOpen(CREATING_SEM, 1);
//    semOpen(PRINTER_SEM, 1);
//    semWait(CREATING_SEM);
//
//    while(1) {
//        semWait(PRINTER_SEM);
//        for(int i = 0 ; i < total; i++){
//            if(state[i] == 1) print(" E ");
//            else print(" . ");
//        }
//        println("");
//        semPost(PRINTER_SEM);
////        for (uint64_t i = 0; i < 300000; i++);
//    }
//}