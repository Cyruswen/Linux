#include "msgqueuetest.h"

int main(){
    pid_t pid;
    //buf用来给子进程接受消息
    char buf[1024];
    //创建一个消息队列
    int msgid = createMsgQueue();
    //创建一个子进程
    pid = fork();
    if(pid < 0){
        perror("fork");
        exit(1);
    }else if(pid > 0){
        int ret = sendMsg(msgid, SERVER_TYPE, "Hello, I am LiShuo!");
        if(ret == -1){
            printf("Send to the message queue failed!\n");
        }else{
            printf("Send to the message queue successfully!\n");
            printf("The message send is:%s\n","Hello, I am LiShuo!");
        }
        //父进程阻塞式等待子进程接受消息
        wait(NULL);
        //销毁消息队列
        destroyMsgQueue(msgid);
    }else if(pid == 0){
        recvMsg(msgid, SERVER_TYPE, buf);
        printf("Child: Receiving from the messqueue:%s\n", buf);
        exit(0);
    }
    return 0;
}
