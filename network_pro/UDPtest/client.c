#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char *argv[2]){
    if(argc != 3){
        printf("Usage:%s [ip] [port]",argv[0]);
        return 1;
    }
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0){
        perror("socket");
        return 2;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));
    server.sin_addr.s_addr = inet_addr(argv[1]);

    char buf[1024];
    struct sockaddr_in peer;
    while(1){
        socklen_t len = sizeof(peer);
        printf("please Enter# ");
        fflush(stdout);
        ssize_t s = read(0, buf, sizeof(buf) - 1);
        if(s > 0){
            buf[s - 1] = 0;
            sendto(sock, buf, strlen(buf), 0, (struct sockaddr*)&server, sizeof(server));
            ssize_t _s = recvfrom(sock, buf, sizeof(buf) - 1, 0, (struct sockaddr*)&peer, &len);
            if(_s > 0){
                buf[_s] = 0;
                printf("server echo# %s\n", buf);
            }
        }
    }
    close(sock);
    return 0;
}
