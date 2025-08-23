#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/errno.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "unpipc.h"

void server(int readfd, int writefd)
{
  int fd;
  ssize_t n;
  char buf[MAXLINE + 1];

  if ((n = read(readfd, buf, MAXLINE)) == 0) {
    printf("end_of_file while reading pathname\n");
    exit(0);
  }

  buf[n] = '\0';

  if ((fd = open(buf, O_RDONLY)) < 0) {
    snprintf(buf+n, sizeof(buf) -n, ": cannot open, %s\n", strerror(errno));
    n = strlen(buf);
    write(writefd, buf, n);
  } else {
    // open succeeded
    while ((n = read(fd, buf, MAXLINE)) > 0) {
      write(writefd, buf, n);
    }
    close(fd);
  }
}
