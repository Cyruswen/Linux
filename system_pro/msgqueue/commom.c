#include "commom.h"
#include <stdio.h>
#include <sys/msg.h>
#include <string.h>

int MsgCommon(int flags){
   key_t  key = ftok(PATHNAME, PROJ_ID);
   if(key < 0){
       perror("ftok");
       return -1;
   }
   //IPC_CREAT:不存在就创建，存在就打开
   //IPC_EXCL:搭配IPC_CREAT使用，消息队列存在，就打开失败
   //句柄
   int msgid = msgget(key, flags);
   if(msgid < 0){
       perror("msgget");
       return -1;
   }
   //此处要注意，不要返回0
   return msgid;
}

int MsgCreate(){
    return MsgCommon(IPC_CREAT | IPC_EXCL | 0666);
}

int MsgOpen(){
    return MsgCommon(IPC_CREAT);
}

int MsgDestroy(int msgid){
    int ret = msgctl(msgid, IPC_RMID, NULL);
    if(ret == -1){
        perror("msgctl IPC_RMID");
        return -1;
    }
    return 0;
}

int MsgSend(int msgid, int type, char* buf, size_t size){
    Msgbuf msgbuf;
    msgbuf.type = type;
    if(size >= sizeof(msgbuf.mtext)/sizeof(msgbuf.mtext[0])){
        printf("buf size is too large\n");
        return -1;
    }
    strcpy(msgbuf.mtext, buf);
    int ret = msgsnd(msgid, &msgbuf, sizeof(msgbuf.mtext), 0);
    if(ret < 0){
        perror("msgsnd");
        return -1;
    }
    return 0;
}

int MsgRcv(int msgid, int type, char* buf, size_t max_size){
   Msgbuf msgbuf;
   int ret = msgrcv(msgid, &msgbuf, sizeof(msgbuf.mtext), type, 0);
   if(ret < 0){
       perror("msgrcv");
       return -1;
   }
   //此处我们的判断依据是MSgSend得来的
   //由于 Send函数能够保证在text中存有\0
   //所以我们不必预留\0
   if(max_size <= sizeof(msgbuf.mtext)){
        printf("output buf size too small\n");
        return -1;
   }
   strcpy(buf,msgbuf.mtext);
   return 0;
}

#define TEST
#ifdef TEST
void TestCreate(){
    int msgid = MsgCreate();
    printf("%d\n",msgid);
}

int main(){
    TestCreate();
    return 0;
}
#endif
