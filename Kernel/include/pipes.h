#ifndef PIPES_H
#define PIPES_H

#include <memoryManagement.h>
#include <queue.h>
#include <scheduler.h>

int pipeOpen(int fd[2]);

int pipeClose(int fd);

int pipeRead(int fd, int length, char *buffer);

int pipeWrite(int fd, int length, char *buffer);

int fillPipeInfo(char *buffer);

#endif //PIPES_H
