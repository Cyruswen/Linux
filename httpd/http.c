#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <pthread.h>
#define MAXSIZE 1024


void usage(){
    printf("usage:./server [ip] [port]\n");
}

int StartUp(char* ip, int port){
    if(ip == NULL){
        exit(-1);
    }
    int opt;
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    //不用输ip
    if(strncmp(ip, "any", 3)){
        addr.sin_addr.s_addr = INADDR_ANY;
    }else{
        addr.sin_addr.s_addr = inet_addr(ip);
    }
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd < 0){
        perror("socket");
        exit(-2);
    }
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if(bind(fd, (struct sockaddr*)&addr, sizeof(addr) < 0)){
        perror("bind");
        exit(-3);
    }
    if(listen(fd, 10) < 0){
        perror("listen");
        exit(-4);
    }
    return fd;
}

int get_line(int sock, char* buf, int size){
    if(buf == NULL){
        return -1;
    }
    //如果s的发送缓冲中没有数据或者数据被协议成功发送完毕后，recv先检查套
    //接字s的接收缓冲区，如果s接收缓冲区中没有数据或者协议正在接收数据，那
    //么recv就一直等待，直到协议把数据接收完毕。当协议把数据接收完毕，recv
    //函数就把s的接收缓冲中的数据copy到buf中（注意协议接收到的数据可能大于
    //buf的长度，所以在这种情况下要调用几次recv函数才能把s的接收缓冲中的数
    //据copy完。recv函数仅仅是copy数据，真正的接收数据是协议来完成的）；
    //recv函数的返回值为实际copy的字节数
    //MSG_PEEK 窥看外来消息。
    int i = 0;
    ssize_t _s = -1;
    char ch = '\0';
    while(i < size - 1 && ch != '\n'){
        _s = recv(sock, &ch, 1, 0);
        if(_s > 0){
            if(ch == '\r'){
                if((_s=recv(sock, &ch, 1, MSG_PEEK))){
                    if(_s > 0 && ch == '\n'){
                        recv(sock, &ch, 1, 0);
                    }
                }
            }
            buf[i++] = ch;
        }
        else{
            buf[i++] = '\n';
            break;
        }
    }
    buf[i] = '\0';
    return i;
}

void accept__request(int sock){
  char buf[MAXSIZE] = {0}; 
  char method[MAXSIZE/2];
  char url[MAXSIZE];
  char path[MAXSIZE];
  memset(buf,'\0',sizeof(buf));
  memset(method,'\0',sizeof(method));
  memset(url, '\0',sizeof(url));
  memset(path, '\0', sizeof(path));
  if()
}

void* handle_client(void* arg){
    int* sock = (int*)arg;
    accept__request(*sock);
    return NULL;
}

int main(int argc, char* argv[]){
    if(argc != 3){
        usage();
        return 1;
    } 
    char* ip = argv[1];
    int port = atoi(argv[2]);
    int listen_socket = StartUp(ip, port);
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);
    fflush(stdout);
    while(1){
        int client_fd = accept(listen_socket, (struct sockaddr*)&client_addr, &len);
        if(client_fd < 0){
            printf("no client\n");
            fflush(stdout);
            continue;
        }
        pthread_t tid;
        //利用子线程对接受到的套接字进行处理
        pthread_create(&tid, NULL, handle_client, (void*)&client_fd);
        pthread_detach(tid);
    }
    return 0;
}
