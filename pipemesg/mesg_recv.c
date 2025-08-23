// mesg_recv.c
#include "mesg.h"

ssize_t
mesg_recv(int fd, struct mymesg* mptr)
{
  // recv means read from fd into mptr
  // we first read header to get length, then read data
  size_t len;
  ssize_t n;

  if ((n = read(fd, mptr, MESGHDRSIZE)) == 0)
    return 0;  // end of file
  else if (n != MESGHDRSIZE)
    err_quit("Message header, expected %d, got %d", MESGHDRSIZE, n);

  // now read data of len
  if ((len = mptr->mesg_len) > 0) {
    if ((n = read(fd, mptr->mesg_data, len)) != len) {
      err_quit("message data, expected %d, got %d", len, n);
    }
  }
  return len;
}
