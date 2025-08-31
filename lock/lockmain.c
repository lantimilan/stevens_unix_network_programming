// lockmain.c
#include "unpipc.h"

#define SEQFILE "seqno" // filename

void my_lock(int), my_unlock(int);

int main(int argc, char* argv[])
{
  int fd;
  long i, seqno;
  pid_t pid;
  ssize_t n;
  char line[MAXLINE + 1];

  pid = getpid();
  fd = open(SEQFILE, O_RDWR, FILE_MODE);
  // check file exists, in MacOS fd is -1 when file does not exist
  printf("open %s returns fd=%d\n", SEQFILE, fd);

  for (i = 0; i < 20; ++i) {
    my_lock(fd); // lock file

    lseek(fd, 0L, SEEK_SET); // rewind before read
    n = read(fd, line, MAXLINE);
    line[n] = '\0';

    n = sscanf(line, "%ld\n", &seqno);
    printf("%s: pid = %ld, seq# = %ld\n", argv[0], (long) pid, seqno);

    seqno++; // increment seqno

    snprintf(line, sizeof(line), "%ld\n", seqno);
    lseek(fd, 0L, SEEK_SET); // rewind before write
    write(fd, line, strlen(line));

    my_unlock(fd); // unlock file
  }
  exit(0);
}
