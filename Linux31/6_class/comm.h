
#ifndef _COMM_H_
#define _COMM_H_

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define PATH_NAME "/tmp"
#define PROJ_ID 0x6666

#define SERVER_TYPE 1
#define CLIENT_TYPE 2

struct msgbuf{
	long mtype;
	char mtext[128];
};

int createMsgQueue();
int getMsgQueue();
int sendMsg(int msgid, char *msg, int t);
int recvMsg(int msgid, int t, char *msg);
void destroyMsgQueue(int);


#endif
