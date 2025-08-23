// server_main.c
#include "fifo.h"
#include "unpipc.h"

void server(int, int);

int main(int argc, char* argv[])
{
  int readfd, writefd;

  if ((mkfifo(FIFO1, FILE_MODE) < 0) && (errno != EEXIST))
    err_sys("cannot create %s", FIFO1);

  if ((mkfifo(FIFO2, FILE_MODE)) < 0 && (errno != EEXIST)) {
    unlink(FIFO1);
    err_sys("cannot create %s", FIFO2);
  }
  readfd = open(FIFO1, O_RDONLY, 0);
  writefd = open(FIFO2, O_WRONLY, 0);

  server(readfd, writefd);
  exit(0);
}
