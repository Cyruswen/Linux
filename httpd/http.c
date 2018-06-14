#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>

void usage(){
    printf("usage:./server [ip] [port]\n");
}

int StartUp(char* ip, int port){
    if(ip == NULL){
        return -1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if(strncmp(ip, "any", 3)){
        addr.sin_addr.s_addr = INADDR_ANY;
    }else{
        addr.sin_addr.s_addr = inet_addr(ip);
    }
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd < 0){
        perror("socket");
        return -1;
    }

}

int main(int argc, char* argv[]){
    if(argc != 3){
        usage();
        return 1;
    } 
    char* ip = argv[1];
    int port = atoi(argv[2]);
    int listen_socket = StartUp(ip, port);
}
