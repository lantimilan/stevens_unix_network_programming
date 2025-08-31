// incr2.c
// gcc -o incr2 incr2.o error.o
//
// ./incr2 /tmp/cnt 1000
// need at least 1000 iter to see interleave of parent and child counters
#include "unpipc.h"

#define SEM_NAME "mysem"

char ipc_name[100];

const char* px_ipc_name(const char* src) {
  // should use strlcpy and strlcat in prod code
  strcpy(ipc_name, "/tmp/");
  return strcat(ipc_name, src);
}

int main(int argc, char* argv[])
{
  int fd, i, nloop, zero = 0;
  int *ptr;
  sem_t *mutex;

  if (argc != 3)
    err_quit("usage: incr2 <pathname> <#loops>");
  nloop = atoi(argv[2]);

  // open files initialize to 0, map into mem
  fd = open(argv[1], O_RDWR | O_CREAT, FILE_MODE);
  write(fd, &zero, sizeof(int));
  ptr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  close(fd);

  // create, initialize, and unlink semaphore
  mutex = sem_open(px_ipc_name(SEM_NAME), O_CREAT | O_EXCL, FILE_MODE, 1);
  sem_unlink(px_ipc_name(SEM_NAME));

  setbuf(stdout, NULL); // stdout is unbuffered
  if (fork() == 0) { // child process
    for (i = 0; i < nloop; ++i) {
      sem_wait(mutex);
      printf("child: %d\n", (*ptr)++);
      sem_post(mutex);
    }
    exit(0);
  }
  // parent
  for (i = 0; i < nloop; ++i) {
    sem_wait(mutex);
    printf("parent: %d\n", (*ptr)++);
    sem_post(mutex);
  }
  exit(0);
}
