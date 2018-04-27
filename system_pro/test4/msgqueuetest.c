#include "msgqueuetest.h"

//下面的函数将创建消息队列函数封装，static修饰，只供createMsgQueue()函数使用
static int commMsgQueue(int flags){
    //初始化一个消息队列的id值
    key_t _key = ftok(PATHNAME, PROJ_ID);
    if(_key < 0){
        perror("ftok");
        return -1;
    }
    //创建一个消息队列，第二个参数flags为同时使用IPC_CREATE和IPC——EXCL
    //表示创建消息队列，单独使用IPC_CREATE表示获取一个存在的消息队列
    int msgid = msgget(_key, flags);
    if(msgid < 0){
        perror("msgget");
    }
    return msgid;
}

int createMsgQueue(){
    return commMsgQueue(IPC_CREAT|IPC_EXCL|0666);
}

int destroyMsgQueue(int msgid){
    //msfctl函数第二个参数为IPC_RMID表示销毁消息队列
    if(msgctl(msgid, IPC_RMID, NULL) < 0){
        perror("msgctl");
        return -1;
    }
    return 0;
}

int sendMsg(int msgid, int who, const char* msg){
    //who表示消息类型的那个long int长整形
    MsgBuf buf;
    buf.mtype = who;
    //把msg的内容拷贝到buf结构体的mtext中
    strcpy(buf.mtext, msg);

    //msgsnd函数的第二个参数表示一个指针指向要发送的消息
    //第三个参数表示发送消息的长度
    //第四个参数表示队列满时要发生的事情，
    //0表示等待，IPC_NOWAIT表示不等待返回错误
    if(msgsnd(msgid, (void*)&buf, sizeof(buf.mtext), 0) < 0){
        perror("msgsnd");
        return -1;
    }

    return 0;
}

//out数组是输出型参数
int recvMsg(int msgid, int recvType, char out[]){
    MsgBuf buf;
    //msgrcv函数第二个参数是指针，指向要接受的消息
    //第三个参数表示消息长度
    //第四个参数表示接收优先级
    //第四个表示没有消息是要发生的事
    //0表示返回队列第一条消息
    if(msgrcv(msgid, (void*)&buf, sizeof(buf.mtext), recvType, 0) < 0){
        perror("msgrcv");
        return -1;
    }
    //把接受到的消息存到out数组里
    strcpy(out, buf.mtext);
    return 0;
}
