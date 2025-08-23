// mainserver.c
#include "fifo.h"

void server(int, int);

int main(int argc, char* argv[])
{
  int readfifo, writefifo, dummyfd, fd;
  char *ptr, buf[MAXLINE+1], fifoname[MAXLINE];
  pid_t pid;
  ssize_t n;

  // create server fifo
  if ((mkfifo(SERV_FIFO, FILE_MODE) < 0) && (errno != EEXIST))
    err_sys("cannot create %s", SERV_FIFO);

  // open server fifo for read
  readfifo = open(SERV_FIFO, O_RDONLY, 0);
  // keep fifo open after client terminate, do not actually read or write fd
  dummyfd = open(SERV_FIFO, O_WRONLY, 0);

  while ((n = readline(readfifo, buf, MAXLINE)) > 0) {
    if (buf[n-1] == '\n')
      --n; // delete newline
    buf[n] = '\0';

    // request format
    // pid<space>pathname
    // server will read file content of pathname, then write to
    // client's fifo with name /tmp/fifo.<pid>
    if ((ptr = strchr(buf, ' ')) == NULL) {
      err_msg("bad request: %s", buf);
      continue;
    }
    *ptr++ = 0; // null terminate
    pid = atol(buf);
    snprintf(fifoname, sizeof(fifoname), "/tmp/fifo.%ld", (long) pid);
    // open client fifo for writing response
    if ((writefifo = open(fifoname, O_WRONLY, 0)) < 0) {
      err_msg("cannot open: %s", fifoname);
      continue;
    }
    if ((fd = open(ptr, O_RDONLY, 0)) < 0) {
      // tell client that we failed to open file requested.
      snprintf(buf + n, sizeof(buf) - n, ": cannot open %s\n", strerror(errno));
      n = strlen(ptr);
      write(writefifo, ptr, n);
      close(writefifo);
    } else {
      // open file succeeded
      while ((n = read(fd, buf, MAXLINE)) > 0) {
        write(writefifo, buf, n);
      }
      close(fd);
      close(writefifo);
    }
  }
  exit(0);
}
