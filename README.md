# pipe example of unpv2
# 2025-08-22
This program starts a server process (child) and a client process (parent), then have the client read from stdin as a pathname, writes to the pipe.
The server then read pathname form pipe, read file using that pathname, and writes file content to a second pipe.
The client then read file content from the second pipe and writes to stdout.

lib
- error.c

file list
- mainpipe.c
- client.c
- server.c

# popen example
# uses popen stdio api
- mainpopen.c

# fifo (named pipe) example
- mainfifo.c
- client.c
- server.c

# standalone client/server example
- fifo.h
- server_main.c
- server.c
- client_main.c
- client.c


