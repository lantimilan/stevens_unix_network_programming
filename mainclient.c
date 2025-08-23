// mainclient.c
#include "fifo.h"

int main(int argc, char* argv[])
{
  int readfifo, writefifo;
  size_t len;
  ssize_t n;
  char *ptr, fifoname[MAXLINE], buf[MAXLINE];
  pid_t pid;

  pid = getpid();
  snprintf(fifoname, sizeof(fifoname), "/tmp/fifo.%ld", (long) pid);
  if ((mkfifo(fifoname, FILE_MODE) < 0) && (errno != EEXIST))
    err_sys("cannot create %s", fifoname);

  // start buffer with pid and a blank
  snprintf(buf, sizeof(buf), "%ld ", (long) pid);
  len = strlen(buf);
  ptr = buf + len;

  // read pathname
  fgets(ptr, MAXLINE - len, stdin);
  len = strlen(buf); // fgets guarantees null terminate

  // open fifo to server and write pid and pathname to fifo
  writefifo = open(SERV_FIFO, O_WRONLY, 0);
  write(writefifo, buf, len);

  // open our own fifo, block until server opens for write
  readfifo = open(fifoname, O_RDONLY, 0);

  // read from IPC, write to stdout
  while ((n = read(readfifo, buf, MAXLINE)) > 0) {
    write(STDOUT_FILENO, buf, n);
  }
  close(readfifo);
  unlink(fifoname);
  exit(0);
}
