// lock_test.c
#include "unpipc.h"

pid_t
lock_test(int fd, int type, off_t offset, int whence, off_t len)
{
  struct flock lock;

  lock.ltype = type; // F_RDLCK or F_WRLCK
  lock.l_start = offset; // byte offset, relative to l_whence
  lock.l_whence = whence; // SEEK_SET, SEEK_CUR, SEEK_END
  lock.l_len = len; // num bytes (0 means to EOF)

  if (fcntl(fd, F_GETLK, &lock) == -1)
    return -1;

  if (lock.l_type == F_UNLCK)
    return 0; // false, region not locked by another process

  return lock.l_pid; // true, return positive pid of lock owner
}
