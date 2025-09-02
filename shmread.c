// shmread.c
#include "unpipc.h"

int main(int argc, char* argv[])
{
  int i, fd;
  struct stat stat;
  unsigned char c, *ptr;

  if (argc != 2)
    err_quit("usage: shmread <name>");

  // open shm and get size, map
  fd = shm_open(argv[1], O_RDONLY, FILE_MODE);
  fstat(fd, &stat);
  ptr = mmap(NULL, stat.st_size, PROT_READ, MAP_SHARED, fd, 0);
  close(fd);

  for (i = 0; i < stat.st_size; ++i)
    if ((c = *ptr++) != (i % 256))
      err_ret("ptr[%d] = %d", i, c);

  exit(0);
}
