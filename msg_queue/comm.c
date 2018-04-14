#include"comm.h"

//创建或访问一个消息队列
static int commMsgQueue(int flags){
    key_t _key = ftok(PATHNAME, PROJ_ID);//创建一个_key

    if(_key < 0){
        perror("ftok");
        return -1;
    }

    int msgid = msgget(_key, flags);//创建或者访问一个消息队列一个消息队列
    if(msgid < 0){
        perror("msgget");
    }
    return msgid;
}

//IP_CREAT和IPC——EXCL同时使用表示创建一个消息队列，IPC_CREAT单独使用表示访问一个消息队列
int createMsgQueue(){
    return commMsgQueue(IPC_CREAT|IPC_EXCL|0666);//创建一个消息队列，权限为0666
}

int getMsgQueue(){
    return commMsgQueue(IPC_CREAT);
}

int destroyMsgQueue(int msgid){
    if(msgctl(msgid, IPC_RMID,NULL) < 0){ //IPC_RMID表示删除队列
        perror("msgctl");
        return -1;
    }
    return 0;
}

int sendMsg(int msgid, int who, char* msg){  //who表示一个长整数，接受者根据这个长整数确定消息类型
    struct msgbuf buf;
    buf.mtype = who;
    strcpy(buf.mtext,msg);
    
    if(msgsnd(msgid, (void*)&buf, sizeof(buf.mtext), 0) < 0){
        perror("msgsnd");
        return -1;
    }

    return 0;
}

int recvMsg(int msgid, int recvType, char out[]){
    struct msgbuf buf;
    if(msgrcv(msgid, (void*)&buf, sizeof(buf.mtext),recvType,0) < 0){
        perror("msgrcv");
        return -1;
    }
    strcpy(out,buf.mtext);
    return 0;
}
