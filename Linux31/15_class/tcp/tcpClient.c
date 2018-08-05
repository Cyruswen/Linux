
#include "comm.h"

//./tcpClient 127.0.0.1 8080
int main(int argc, char *argv[])
{
	if(argc != 3){
		printf("Usage: %s [ip] [port]\n", argv[0]);
		return 1;
	}
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0){
		printf("socket error!\n");
		return 2;
	}

	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port  = htons(atoi(argv[2]));
	server.sin_addr.s_addr = inet_addr(argv[1]);

	if(connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0){
		printf("connect error!\n");
		return 3;
	}

	char buf[MAX];
//	int x, y;
//	request_t r;
	while(1){
		printf("please Enter# ");
//		scanf("%d%d", &r.x, &r.y);
//
//		write(sock, &r, sizeof(r));
//
//		response_t rp;
//		read(sock, &rp, sizeof(rp));
//
//		printf("%d + %d = %d\n",r.x, r.y, rp.res);
//		fflush(stdout);
		ssize_t s = read(0, buf, sizeof(buf)-1);
		if(s > 0){
			buf[s-1] = 0;
			if(strcmp("quit", buf) == 0){
				printf("client quit!\n");
				break;
			}
			write(sock, buf, strlen(buf));
		//	s = read(sock, buf, sizeof(buf)-1);
		//	buf[s] = 0;
		//	printf("Server Echo# %s\n", buf);
		}
	}

	close(sock);
	return 0;
}




