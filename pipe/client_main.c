// client_main.c
#include "fifo.h"
#include "unpipc.h"

void client(int, int);

int main(int argc, char* argv[])
{
  int readfd, writefd;

  // must open writefd first because server opens readfd first and blocked on this
  writefd = open(FIFO1, O_WRONLY, 0);
  readfd = open(FIFO2, O_RDONLY, 0);

  client(readfd, writefd);

  // server already exit
  close(readfd);
  close(writefd);
  unlink(FIFO1);
  unlink(FIFO2);
  exit(0);
}
