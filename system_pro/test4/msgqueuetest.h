#pragma once
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <string.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define PATHNAME "."
#define PROJ_ID 0x6666

#define SERVER_TYPE 1
//#define CLIENT_TYPE 2

typedef struct MsgBuf{
    long mtype;
    char mtext[1024];
} MsgBuf;

int createMsgQueue();

int destroyMsgQueue(int msgid);

int sendMsg(int msgid, int who, const char* msg);

int recvMsg(int msgid, int recvType, char out[]);
