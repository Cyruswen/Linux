#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define MAX 1024

int main(int argc, char *argv[]){
    if(argc != 3){
        printf("Usage: %s [ip] [port]\n",argv[0]);
        return 1;
    }
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0){
        printf("socket error!\n");
        return 2;
    }

    struct sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_port = htons(atoi(argv[2]));
    service.sin_addr.s_addr = inet_addr(argv[1]);

    if(connect(sock, (struct sockaddr*)&service, sizeof(service)) < 0){
        printf("connect error!\n");
        return 3;
    }
    
    char buf[MAX];
    while(1){
        printf("Please Enter# ");
        fflush(stdout);
        ssize_t s = read(0, buf, sizeof(buf) - 1);
        if(s > 0){
            buf[s - 1] = 0;
            if(strcmp("quit", buf) == 0){
                printf("Client quit!\n");
                break;
            }
            write(sock, buf, strlen(buf));
            s = read(sock, buf, sizeof(buf) - 1 );
            buf[s] = 0;
            printf("Service Echo# %s\n",buf);
        }
    }
    close(sock);
    return 0;
}
