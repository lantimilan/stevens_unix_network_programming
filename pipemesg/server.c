// server.c
#include "mesg.h"

void
server(int readfd, int writefd)
{
  FILE *fp;
  ssize_t n;
  struct mymesg mesg;

  // read path name from ipc channel
  mesg.mesg_type = 1;
  if ((n = mesg_recv(readfd, &mesg)) == 0)
    err_quit("path name missing");
  mesg.mesg_data[n] = '\0';  // null terminating

  if ((fp = fopen(mesg.mesg_data, "r")) == NULL) {
    snprintf(mesg.mesg_data + n, sizeof(mesg.mesg_data) - n, ": cannot open %s\n", strerror(errno));
    mesg.mesg_len = strlen(mesg.mesg_data);
    mesg_send(writefd, &mesg);
  } else {
    // fopen success, now read file
    while (fgets(mesg.mesg_data, MAXMESGDATA, fp) != NULL) {
      mesg.mesg_len = strlen(mesg.mesg_data);
      mesg_send(writefd, &mesg);
    }
    fclose(fp);
  }
  // send a 0-length message to signal end of message
  mesg.mesg_len = 0;
  mesg_send(writefd, &mesg);
}
