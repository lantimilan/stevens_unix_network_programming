// incr3.c
// use shared memory semephore instead of named semaphore
#include "unpipc.h"

struct shared {
  sem_t mutex;
  int count;
} shared;

int main(int argc, char* argv[])
{
  int fd, i, nloop;
  struct shared *ptr;

  if (argc != 3)
    err_quit("usage: incr3 <pathname> <#loops>");
  nloop = atoi(argv[2]);

  // open file, init to 0, map to memory
  fd = open(argv[1], O_RDWR | O_CREAT, FILE_MODE);
  write(fd, &shared, sizeof(struct shared));
  ptr = mmap(NULL, sizeof(struct shared), PROT_READ | PROT_WRITE,
                  MAP_SHARED, fd, 0);
  close(fd);

  // init sem that is shared
  sem_init(&ptr->mutex, 1, 1);
  
  setbuf(stdout, NULL); // stdout is unbuffered
  if (fork() == 0) { // child
    for (i = 0; i < nloop; i++) {
      sem_wait(&ptr->mutex);
      printf("child %d\n", ptr->count++);
      sem_post(&ptr->mutex);
    }
    exit(0);
  }
  // parent
  for (i = 0; i < nloop; i++) {
    sem_wait(&ptr->mutex);
    printf("parent %d\n", ptr->count++);
    sem_post(&ptr->mutex);
  }
  exit(0);
}
