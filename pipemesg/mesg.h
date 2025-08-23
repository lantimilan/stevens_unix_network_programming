// mesg.h
#include "unpipc.h"

/** our own messages to use with pipes, fifos and message queues */

/** want sizeof(struct mymesg) <= PIPE_BUF for atomic write */
#define MAXMESGDATA (PIPE_BUF - 2*sizeof(long))

/** length of mesg_len and mesg_type */
#define MESGHDRSIZE (sizeof(struct mymesg) - MAXMESGDATA)

struct mymesg {
  long mesg_len;  // bytes in mesg_data, can be 0
  long mesg_type; // must be > 0
  char mesg_data[MAXMESGDATA];
};

ssize_t mesg_send(int fd, struct mymesg*);
ssize_t mesg_recv(int fd, struct mymesg*);
