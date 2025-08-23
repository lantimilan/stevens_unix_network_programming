// unpipc.h
#ifndef __UNPIPC_H
#define __UNPIPC_H

#define MAXLINE 4096

#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

void err_sys(const char *, ...);

// for printf
#include <stdio.h>

// for exit
#include <stdlib.h>

// for unlink
#include <unistd.h>

// for mkfifo
#include <sys/types.h>
#include <sys/stat.h>

// for open
#include <fcntl.h>

// for errno
#include <sys/errno.h>

#endif

