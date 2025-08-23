// client.c
#include "mesg.h"

void
client(int readfd, int writefd)
{
  size_t len;
  ssize_t n;
  struct mymesg mesg;

  // read path name
  fgets(mesg.mesg_data, MAXMESGDATA, stdin);
  len = strlen(mesg.mesg_data);
  if (mesg.mesg_data[len-1] == '\n')
    --len; // remove trailing newline
  mesg.mesg_len = len;
  mesg.mesg_type = 1;  // type must be > 0

  // write path name to ipc channel
  mesg_send(writefd, &mesg);

  // read from ipc channel
  while ((n = mesg_recv(readfd, &mesg)) > 0)
    write(STDOUT_FILENO, mesg.mesg_data, n);
}
