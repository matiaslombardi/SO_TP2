// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <phylo.h>
#include <syscalls.h>
#include <string.h>
#include <standardIO.h>

#define MAX_PHYLOS 8
#define MIN_PHYLOS 2
#define CREATING_SEM "createPhylo"
#define FORK_SEM "forkSem"
#define STATE_CHANGING_SEM "stateSem"

#define ALERT_COLOR 0xfc3d03
#define SUCCESS_COLOR 0x19a600
#define PHYLO_COLOR 0xf2f2f2

static int total = MIN_PHYLOS;
static int phyloIdCount = 0;

static uint64_t phylos[MAX_PHYLOS] = {0};
static int phylosPid[MAX_PHYLOS] = {0};
static uint64_t state[MAX_PHYLOS] = {0};
static char sems[MAX_PHYLOS][10] = {{0}};

void phylo(int id);

void printPhylos();

static void createPhylo();

static void addPhylo();

static void removePhylo();

void initPhylos() {
    semOpen(CREATING_SEM, 1);
    semOpen(STATE_CHANGING_SEM, total);

    semWait(CREATING_SEM);
    print("");

    for (int i = 0; i < total; i++) {
        createPhylo();
    }
    print("");

    println("Welcome to Dining Philosophers. Press 'a' for add, press 'r' for remove, press 'q' for quit.");

    for (int i = 0; i < total; i++) {
        semPost(CREATING_SEM);
        print("");
    }
    print("");

    char c;
    while ((c = getChar()) != 'q') {
        if (c != 0) {
            switch (c) {
                case 'a':
                    if (total < MAX_PHYLOS) {
                        addPhylo();
                    } else {
                        printcln("No more seats.", ALERT_COLOR);
                    }
                    break;
                case 'r':
                    if (total > MIN_PHYLOS) {
                        removePhylo();
                    } else {
                        printcln("Minimun seats.", ALERT_COLOR);
                    }
                    break;
            }
        }
    }

    for (int i = 0; i < total; i++) {
        semClose(sems[i]);
        killProcess(phylosPid[i]);
    }

    semClose(CREATING_SEM);
    semClose(STATE_CHANGING_SEM);

    total = MIN_PHYLOS;
    phyloIdCount = 0;

    for (int i = 0; i < MAX_PHYLOS; i++) {
        phylos[i] = 0;
        phylosPid[i] = 0;
        state[i] = 0;
        sems[i][0] = 0;
    }

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
    phylosPid[phyloIdCount] = createProcess((uint64_t * ) & phylo, 0, 0, 1, phyloIdCount, 0, 0, "phylo");
    phyloIdCount++;
}

static void addPhylo() {
    for (int i = 0; i < total; i++) {
        semWait(STATE_CHANGING_SEM);
    }

    createPhylo();

    printc("Phylo added\n", SUCCESS_COLOR);


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
    printc("Phylo removed\n", SUCCESS_COLOR);

    for (int i = 0; i < total; i++) {
        semPost(STATE_CHANGING_SEM);
    }
}

void phylo(int id) {
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
            if (state[i] == 1) printc("E", PHYLO_COLOR);
            else printc(".", PHYLO_COLOR);
        }
        println("");

        for (uint64_t i = 0; i < 50000000; i++);

        state[id] = 0;

        semPost(second);
        semPost(first);

        semPost(STATE_CHANGING_SEM);
    }
}