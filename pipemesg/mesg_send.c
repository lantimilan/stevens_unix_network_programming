// mesg_send.c
#include "mesg.h"

ssize_t
mesg_send(int fd, struct mymesg* mptr)
{
  // send means write mymesg to fd
  return write(fd, mptr, MESGHDRSIZE + mptr->mesg_len);
}
