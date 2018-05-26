#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define MAX 1024

int startup(char* ip, int port){
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0){
        printf("sock error!\n");
        exit(2);
    }
    struct sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = inet_addr(ip);
    local.sin_port = htons(port);

    if(bind(sock, (struct sockaddr*)&local, sizeof(local)) < 0){
        printf("bind error!\n");
        exit(3);
    }

    if(listen(sock, 5) < 0){
        printf("listen error!\n");
        exit(4);
    }
    
    return sock;
}

void service(int sock, char* ip, int port){
    char buf[MAX];
    while(1){
        buf[0] = 0;
        ssize_t s = read(sock, buf, sizeof(buf)-1);
        if(s > 0){
            buf[s] = 0;
            printf("[%s:%d] says: %s\n",ip, port, buf);
            //TODO
            write(sock, buf, strlen(buf));
        }
        else if(s == 0){
            printf("client [%s:%d] quit!\n",ip, port);
            break;
        }
        else if(s <  0){
            printf("read error!\n");
            break;
        }
    }
}

int main(int argc, char* argv[]){
    if(argc != 3){
        printf("Usage: %s [ip][port]\n",argv[0]);
        return 1;
    }
    int listen_sock = startup(argv[1],atoi(argv[2]));

    struct sockaddr_in peer;
    char ipbuf[24];
    for(;;){
        ipbuf[0] = 0;
        socklen_t len = sizeof(peer);
        int new_sock = accept(listen_sock, (struct sockaddr*)&peer, &len);
        if(new_sock < 0){
            printf("accept error!\n");
            continue;
        }
        inet_ntop(AF_INET, &peer.sin_addr, ipbuf, sizeof(ipbuf));
        printf("get a new connect,[%s:%d]\n",ipbuf, ntohs(peer.sin_port));
        service(new_sock, ipbuf, peer.sin_port);;
        close(new_sock);
    }
    return 0;
}
