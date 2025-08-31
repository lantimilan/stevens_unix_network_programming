// test2.c
// test read write lock priority
#include "unpipc.h"
#include "lockmacro.h"

int gf_time();

int main(int argc, char* argv[])
{
  int fd;
  fd = open("test1.data", O_RDWR | O_CREAT, FILE_MODE);

  read_lock(fd, 0, SEEK_SET, 0); // parent read locks entire file
  printf("%d: parent has read lock\n", gf_time());

  if (fork() == 0) { // first child
    exit(0);
  }
  if (fork() == 0) { // second child
    exit(0);
  }

  // parent
  sleep(5);
  un_lock(fd, 0, SEEK_SET, 0);
  printf("%d: parent releases read lock\n", gf_time());
  exit(0);

}

int gf_time()
{
  return gettimeofday(NULL, NULL);
}
