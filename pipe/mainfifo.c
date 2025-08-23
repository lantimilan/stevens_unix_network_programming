// mainfifo.c
// for printf
#include <stdio.h>

// for exit
#include <stdlib.h>

// for unlink
#include <unistd.h>

// for mkfifo
#include <sys/types.h>
#include <sys/stat.h>

// for open
#include <fcntl.h>

// for errno
#include <sys/errno.h>

#include "unpipc.h"

#define FIFO1 "/tmp/fifo.1"
#define FIFO2 "/tmp/fifo.2"

// forward declaration
void client(int, int), server(int, int);

int main(int argc, char* argv[])
{
  int readfd, writefd;
  pid_t childpid;

  if ((mkfifo(FIFO1, FILE_MODE) < 0) && (errno != EEXIST)) {
    printf("cannot create %s", FIFO1);
    exit(1);
  }
  if ((mkfifo(FIFO2, FILE_MODE) < 0) && (errno != EEXIST)) {
    unlink(FIFO1);
    printf("cannot create %s", FIFO2);
    exit(1);
  }

  if ((childpid = fork()) == 0) { // child
    readfd = open(FIFO1, O_RDONLY, 0);
    writefd = open(FIFO2, O_WRONLY, 0);

    server(readfd, writefd);
    exit(0);
  }

  // parent
  // must open writefd first, open readfd first will deadlock
  writefd = open(FIFO1, O_WRONLY, 0);
  readfd = open(FIFO2, O_RDONLY, 0);

  client(readfd, writefd);

  waitpid(childpid, NULL, 0);  // wait for child to terminate

  close(readfd);
  close(writefd);
  
  unlink(FIFO1);
  unlink(FIFO2);
  exit(0);
}
