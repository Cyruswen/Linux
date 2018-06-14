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

void accept__request(int sock){
    
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
