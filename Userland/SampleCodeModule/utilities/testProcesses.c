// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <testUtil.h>
#include <stdint.h>
#include <standardIO.h>
#include <syscalls.h>
#include <string.h>
 #include <tests.h>


//TO BE INCLUDED
void processesEndless() {
    while (1);
}

uint64_t processesCreateProcess(uint64_t *entryPoint) {
    return createProcess(entryPoint, 0, 0, 1, 0, 0, 0, "processTester");
}

uint64_t processesKill(uint64_t pid) {
    killProcess(pid);
    return 0;
}

uint64_t processesBlock(uint64_t pid) {
    changeProcessState(pid);
    return 0;
}

uint64_t processesUnblock(uint64_t pid) {
    changeProcessState(pid);
    return 0;
}

#define MAX_PROCESSES 100 //Should be around 80% of the the processes handled by the kernel

enum State {
    ERROR, RUNNING, BLOCKED, KILLED
};

typedef struct P_rq {
    uint32_t pid;
    enum State state;
} p_rq;

void testProcesses() {
    p_rq p_rqs[MAX_PROCESSES];
    uint8_t rq;
    uint8_t alive = 0;
    uint8_t action;

    while (1) {
        // Create MAX_PROCESSES processes
        for (rq = 0; rq < MAX_PROCESSES; rq++) {
            p_rqs[rq].pid = processesCreateProcess((uint64_t * ) & processesEndless);  // TODO: Port this call as required

            //TODO si el malloc de crear procesos es null, devolver -1
            if (p_rqs[rq].pid == -1) {                           // TODO: Port this as required
                print("Error creating process\n");               // TODO: Port this as required
                return;
            } else {
                p_rqs[rq].state = RUNNING;
                alive++;
            }
        }

        // Randomly kills, blocks or unblocks processes until every one has been killed
        while (alive > 0) {

            for (rq = 0; rq < MAX_PROCESSES; rq++) {
                action = GetUniform(2) % 2;

                switch (action) {
                    case 0:
                        if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == BLOCKED) {
                            if (processesKill(p_rqs[rq].pid) == -1) {          // TODO: Port this as required
                                print("Error killing process\n");        // TODO: Port this as required
                                return;
                            }
                            p_rqs[rq].state = KILLED;
                            alive--;
                        }
                        break;

                    case 1:
                        if (p_rqs[rq].state == RUNNING) {
                            if (processesBlock(p_rqs[rq].pid) == -1) {          // TODO: Port this as required
                                print("Error blocking process\n");       // TODO: Port this as required
                                return;
                            }
                            p_rqs[rq].state = BLOCKED;
                        }
                        break;
                }
            }

            // Randomly unblocks processes
            for (rq = 0; rq < MAX_PROCESSES; rq++)
                if (p_rqs[rq].state == BLOCKED && GetUniform(2) % 2) {
                    if (processesUnblock(p_rqs[rq].pid) == -1) {            // TODO: Port this as required
                        print("Error unblocking process\n");         // TODO: Port this as required
                        return;
                    }
                    p_rqs[rq].state = RUNNING;
                }
        }
    }
    _exit(1);
}

//int main() {
//    test_processes();
//    return 0;
//}
