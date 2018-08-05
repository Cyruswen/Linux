#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

//udp_client 127.0.0.1 8080
int main(int argc, char *argv[])
{
	if(argc != 3){
		printf("Usage: %s [ip] [port]\n", argv[0]);
		return 1;
	}

	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0){
		printf("socket error!\n");
		return 2;
	}

	char buf[128];
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[2]));
	server.sin_addr.s_addr = inet_addr(argv[1]);
	socklen_t len = sizeof(server);

	for(;;){
		buf[0] = 0;
		ssize_t s = read(0, buf, sizeof(buf)-1);
		if(s > 0){
			buf[s-1] = 0;
			sendto(sock, buf, strlen(buf), 0,\
					(struct sockaddr*)&server, len);
			s = recvfrom(sock, buf, sizeof(buf)-1, 0, NULL, NULL);
			if(s > 0){
				buf[s] = 0;
				printf("server echo# %s\n", buf);
			}
		}
	}

	close(sock);
	return 0;
}














