// signal_user.c
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdarg.h>

#define MAXLINE 1024

static void err_doit(int, int, const char *fmt, va_list ap);
static void err_sys(const char*, ...) __attribute__((noreturn));
static void err_dump(const char*, ...) __attribute__((noreturn));
static void sig_usr(int);

int main(void)
{
  if (signal(SIGUSR1, sig_usr) == SIG_ERR)
    err_sys("cannot catch SIGUSR1");
  if (signal(SIGUSR2, sig_usr) == SIG_ERR)
    err_sys("cannot catch SIGUSR2");

  for ( ; ; )
    pause();
}

static void
sig_usr(int signo)
{
  if (signo == SIGUSR1)
    printf("received SIGUSR1\n");
  else if (signo == SIGUSR2)
    printf("received SIGUSR2\n");
  else
    //err_dump("received signal %d\n", signo);
    printf("received signal %d\n", signo);
}

/*
* fatal error related to a sys call
* print a message and terminate
*/
static void
err_sys(const char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  err_doit(1, errno, fmt, ap);
  va_end(ap);
  exit(1);
}

/*
* fatal error related to a syscall
* print message, dump core and terminate.
*/
static void
err_dump(const char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  err_doit(1, errno, fmt, ap);
  va_end(ap);

  abort();
  exit(1);
}

static void
err_doit(int errnoflag, int error, const char *fmt, va_list ap)
{
  char buf[MAXLINE];
  
  vsnprintf(buf, MAXLINE - 1, fmt, ap);
  if (errnoflag)
    snprintf(buf + strlen(buf), MAXLINE - strlen(buf) -1, ": %s", strerror(error));
  strcat(buf, "\n");
  fflush(stdout);
  fputs(buf, stderr);
  fflush(NULL);
}
