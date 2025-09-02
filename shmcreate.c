// shmcreate.c
#include "unpipc.h"

int main(int argc, char* argv[])
{
  int c, fd, flags;
  char *ptr;
  off_t length;
  const char *name;

  flags = O_RDWR | O_CREAT;
  while ((c = getopt(argc, argv, "e")) != -1) {
    switch (c) {
    case 'e':
      flags |= O_EXCL;
      break;
    }
  }
  if (optind != argc - 2) // must have two args, name and length
    err_quit("usage: shmcreate [ -e ] <name> <length>");

  length = atoi(argv[optind + 1]);
  name = argv[optind];

  fd = shm_open(name, flags, FILE_MODE);
  ftruncate(fd, length);

  // NULL means system can find an addr with enough space
  ptr = mmap(/*addr=*/NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  exit(0);

}
