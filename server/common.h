#ifndef _COMMON_H
#define _COMMON_H

#include <bits/stdc++.h>
// #include<studio.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdarg.h>

#include <errno.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <strings.h>
#include <sys/ioctl.h>
#include <sys/time.h>

#include <signal.h>
// #include<studio.h>
#include <stdlib.h>
#include <netdb.h>

#define SERVER_PORT 18000

#define MAX_LENGHT 4096 // max length of reading data

#define SERVER_FILE_ROOT "/root/intern/garbage/multithreaded_server/static/sfiles/"

#define SA struct sockaddr

// using namespace std ;
// void error_and_kill(const char *fmt , ...) ;

 char *bin2hex(const unsigned char * input  , size_t len) ;

#endif