// lock_reg.c
#include "unpipc.h"

int
lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len)
{
  struct flock lock;

  lock.l_type = type; // F_RDLCK, F_WRLCK, F_UNLCK
  lock.l_start = offset; // byte offset, relative to whence
  lock.l_whence = whence; // SEEK_SET, SEEK_CUR, SEEK_END
  lock.l_len = len; // num bytes, 0 means EOF

  return fcntl(fd, cmd, &lock); // -1 upon error
}
