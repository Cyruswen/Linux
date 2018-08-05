#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define SIZE 128

void service(int sock)
{
	char buf[SIZE];
	for(;;){
		struct sockaddr_in peer;
		socklen_t len = sizeof(peer);
		buf[0] = 0;
		ssize_t s = recvfrom(sock, buf, sizeof(buf)-1, 0, \
				(struct sockaddr*)&peer, &len);
		if( s > 0 ){
			buf[s] = 0;
			printf("[%s:%d]# %s\n", inet_ntoa(peer.sin_addr),\
					ntohs(peer.sin_port), buf);

			sendto(sock, buf, strlen(buf), 0, \
					(struct sockaddr*)&peer, len);
		}
	}
}

//./udp_server 192.168.3.55 8080
int main(int argc, char *argv[])
{
	if(argc != 3){
		printf("Usage: %s [ip] [port]\n", argv[0]);
		return 3;
	}
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0){
		printf("socket error!\n");
		return 1;
	}

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(atoi(argv[2]));
	local.sin_addr.s_addr = inet_addr(argv[1]);

	if(bind(sock, (struct sockaddr*)&local, sizeof(local)) < 0){
		printf("bind error!\n");
		return 2;
	}

	service(sock);

	close(sock);
	return 0;
}















