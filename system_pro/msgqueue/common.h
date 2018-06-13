#pragma once
#include <stddef.h>
#define PATHNAME "."
#define PROJ_ID 1

typedef struct Msgbuf {
    long type; 
    char mtext[1024];
} Msgbuf;

int MsgCreate();

int MsgOpen();

int MsgDestroy(int msgid);

int MsgSend(int msgid, int type, char* buf, size_t size);

int MsgRcv(int msgid, int type, char* buf, size_t max_size);
