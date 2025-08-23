// mainpopen.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "unpipc.h"

int main(int argc, char* argv[])
{
  size_t n;
  char buf[MAXLINE], command[MAXLINE];
  FILE *fp;

  fgets(buf, MAXLINE, stdin);
  n = strlen(buf);

  if (buf[n-1] == '\n')
    buf[n-1] = '\0';
  snprintf(command, sizeof(command), "cat %s", buf);

  fp = popen(command, "r");

  // copy from pipe to stdout
  while (fgets(buf, MAXLINE, fp) != NULL) {
    fputs(buf, stdout);
  }
  fclose(fp);
  exit(0);
}
