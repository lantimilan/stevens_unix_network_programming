// client.c
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "unpipc.h"

void client(int readfd, int writefd)
{
  size_t len;
  ssize_t n;
  char buf[MAXLINE];

  fgets(buf, MAXLINE, stdin);
  len = strlen(buf);
  if (buf[len-1] == '\n')
    --len;

  // write pathname to ipc channel
  write(writefd, buf, len);

  // read from IPC, write to stdout
  while ((n = read(readfd, buf, MAXLINE)) > 0)
    write(STDOUT_FILENO, buf, n);
}
