// test3.c
// test read write lock priority
#include "unpipc.h"
#include "lockmacro.h"

const char* gf_time();

int main(int argc, char* argv[])
{
  int fd;
  fd = open("test1.data", O_RDWR | O_CREAT, FILE_MODE);

  write_lock(fd, 0, SEEK_SET, 0); // parent read locks entire file
  printf("%s: parent has write lock\n", gf_time());

  if (fork() == 0) { // first child
    sleep(1);
    printf("%s: first child tries to obtain write lock\n", gf_time());
    writew_lock(fd, 0, SEEK_SET, 0);
    printf("%s: first child obtains write lock\n", gf_time());
    sleep(2);
    un_lock(fd, 0, SEEK_SET, 0);
    printf("%s: first child releases write lock\n", gf_time());
    exit(0);
  }
  if (fork() == 0) { // second child
    sleep(3);
    printf("%s: second child tries to obtain read lock\n", gf_time());
    readw_lock(fd, 0, SEEK_SET, 0);
    printf("%s: second child obtains read lock\n", gf_time());
    sleep(4);
    un_lock(fd, 0, SEEK_SET, 0);
    printf("%s: second child releases read lock\n", gf_time());
    exit(0);
  }

  // parent
  sleep(5);
  un_lock(fd, 0, SEEK_SET, 0);
  printf("%s: parent releases write lock\n", gf_time());
  exit(0);
}

const char* gf_time()
{
  static char buf[1024];
  struct timeval tv;
  time_t nowtime;
  struct tm* nowtm;

  gettimeofday(&tv, NULL);
  nowtime = tv.tv_sec;
  nowtm = localtime(&nowtime);

  strftime(buf, sizeof(buf), "%H:%M:%S", nowtm);
  return buf;
}

int gf_time2()
{
  return gettimeofday(NULL, NULL);
}
