
#ifndef _COMM_H_
#define _COMM_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define MAX 128

typedef struct {
	int x;
	int y;
}request_t;

typedef struct{
	int res;
}response_t;

#endif
