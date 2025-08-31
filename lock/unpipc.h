// unpipc.h
#ifndef __UNPIPC_H
#define __UNPIPC_H

// for pthread
#include <pthread.h>

// for printf
#include <stdio.h>

// for size_t
#include <stddef.h>

// for exit
#include <stdlib.h>

// for unlink
#include <unistd.h>

// for strlen
#include <string.h>

// for gettimeofday
#include <sys/time.h>

// for mkfifo
#include <sys/types.h>
#include <sys/stat.h>

// for mmap
#include <sys/mman.h>

// for semaphore
#include <semaphore.h>

// for open and flock
#include <fcntl.h>
#include <sys/file.h>

// for errno
#include <sys/errno.h>

// after lib includes
#define MAXLINE 4096

#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

// min and max
#define	min(a,b)	((a) < (b) ? (a) : (b))
#define	max(a,b)	((a) > (b) ? (a) : (b))

// set_concurrency
// int		 set_concurrency(int);

// implemented in error.c
void err_sys(const char *, ...);
void err_msg(const char *fmt, ...);
void err_quit(const char *fmt, ...);

// implemented in readline.c
ssize_t readline(int fd, void *vptr, size_t maxlen);

#endif
