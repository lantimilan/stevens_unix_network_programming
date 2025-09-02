// server1.c
// program that creates and initializes shared memory and semaphore
#include "unpipc.h"

const char* px_ipc_name(const char* name)
{
  static char buf[1024];
  strcpy(buf, "/tmp");
  return strcat(buf, name);
}

struct shmstruct {
  int count;
};
sem_t *mutex;

int main(int argc, char* argv[])
{
  int fd;
  struct shmstruct *ptr;
  const char *ipc_name_shm, *ipc_name_sem;

  if (argc != 3)
    err_quit("usage: server1 <shmname> <semname>");

  ipc_name_shm = px_ipc_name(argv[1]);
  ipc_name_sem = px_ipc_name(argv[2]);
  shm_unlink(ipc_name_shm);
  fd = shm_open(ipc_name_shm, O_RDWR | O_CREAT | O_EXCL, FILE_MODE);
  ftruncate(fd, sizeof(struct shmstruct));
  ptr = mmap(NULL, sizeof(struct shmstruct), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  close(fd);

  sem_unlink(ipc_name_sem); // OK if this fails
  mutex = sem_open(ipc_name_sem, O_CREAT | O_EXCL, FILE_MODE, 1);
  sem_close(mutex);

  exit(0);
}

