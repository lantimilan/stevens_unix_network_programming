// fdduplex.c
//
// Note that this does NOT work on linux because linux pipe is unidirectional.
//
//
// for pipe, read
#include <unistd.h>
// for exit
#include <stdlib.h>
// for printf
#include <stdio.h>

int main(int argc, char* argv[])
{
  int fd[2], n;
  char c;
  pid_t childpid;

  pipe(fd);  // assume a full duplex pipe
  if ((childpid = fork()) == 0) { // child
    sleep(3);
    if ((n = read(fd[0], &c, 1)) != 1) {
      printf("child: read returned %d\n", n);
      exit(0);
    }
    printf("child read %c\n", c);
    exit(0);
  }

  // parent
  write(fd[1], "p", 1);
  if ((n = read(fd[1], &c, 1)) != 1) {
      printf("parent: read returned %d\n", n);
      exit(0);
  }
  printf("parent read %c\n", c);
  exit(0);
}
