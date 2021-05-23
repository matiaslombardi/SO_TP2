#include <phylo.h>

#define MAX_PHYLOS 5
#define CREATING_SEM "createPhylo"
#define FORK_SEM "forkSem"
#define PRINTER_SEM "printerSem"

static int total = 5;
static int phyloIdCount = 0;

static uint64_t phylos[MAX_PHYLOS] = {0};
static uint64_t state[MAX_PHYLOS] = {0};
static char sems[MAX_PHYLOS][10] = {{0}};

void phylo(int id);
void printPhylos();

void initPhylos() {
    semOpen(CREATING_SEM, 1);
    semOpen(PRINTER_SEM, 1);

    semWait(CREATING_SEM);

//    createProcess((uint64_t * ) &printPhylos, 0, 0, 1, 0, 0, 0);

    for (int i = 0; i < MAX_PHYLOS; i++) {
        char semName[20] = {0};
        char semNameId[3] = {0};
        itoaTruncate(phyloIdCount, semNameId, 3);
        strcat(semName, FORK_SEM);
        strcat(semName, semNameId);

        semOpen(semName, 1);
        strcpy(sems[phyloIdCount], semName);

        phylos[phyloIdCount] = phyloIdCount;
        createProcess((uint64_t * ) &phylo, 0, 0, 1, phyloIdCount++, 0, 0);
    }

    for(int i = 0; i < MAX_PHYLOS + 1; i++) {
        semPost(CREATING_SEM);
    }

    while(1);
}

void phylo(int id) {
    semOpen(CREATING_SEM, 1);
    semOpen(PRINTER_SEM, 1);
    semWait(CREATING_SEM);

    char *first;
    char *second;
    if (id % 2 == 0) {
        first = sems[(id - 1 + phyloIdCount) % phyloIdCount];
        second = sems[id];

    } else {
        first = sems[id];
        second = sems[id - 1];
    }

    while(1) {
        for (uint64_t i = 0; i < 10000000; i++);

        semWait(first);
        semWait(second);

//        semWait(PRINTER_SEM);
        state[id] = 1;
//        semPost(PRINTER_SEM);

        for(int i = 0 ; i < total; i++){
            if(state[i] == 1) print(" E ");
            else print(" . ");
        }
        println("");
        for (uint64_t i = 0; i < 10000000; i++);

//        semWait(PRINTER_SEM);
        state[id] = 0;
//        semPost(PRINTER_SEM);

        semPost(second);
        semPost(first);

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
