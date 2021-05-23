#ifndef PIPES_H
#define PIPES_H

int pipeOpen(int fd[2]);

int pipeClose(int fd);

int pipeRead(int fd, int length, char * buffer);

int pipeWrite(int fd, int length, char * buffer);

#endif //PIPES_H
