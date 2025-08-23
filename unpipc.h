// unpipc.h
#ifndef __UNPIPC_H
#define __UNPIPC_H

// for printf
#include <stdio.h>

// for size_t
#include <stddef.h>

// for exit
#include <stdlib.h>

// for unlink
#include <unistd.h>

#include <string.h>

// for mkfifo
#include <sys/types.h>
#include <sys/stat.h>

// for open
#include <fcntl.h>

// for errno
#include <sys/errno.h>

// after lib includes
#define MAXLINE 4096

#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

// implemented in error.c
void err_sys(const char *, ...);
void err_msg(const char *fmt, ...);

// implemented in readline.c
ssize_t readline(int fd, void *vptr, size_t maxlen);

#endif

