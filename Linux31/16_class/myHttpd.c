#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

void service(int sock)
{
	char buf[10240];
	read(sock, buf, sizeof(buf));
	const char *response = "HTTP/1.0 200 OK\n\n<html><head><h1>hello bit</h1></head></html>\n";
	write(sock, response, strlen(response));
}

int startup(int port)
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0){
		exit(2);
	}
	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = htonl(INADDR_ANY);
	local.sin_port = htons(port);

	if(bind(sock, (struct sockaddr*)&local, sizeof(local)) < 0){
		exit(3);
	}

	if(listen(sock, 5) < 0){
		exit(4);
	}
	return sock;
}

//./myHttpd 8080
int main(int argc, char *argv[])
{
	if(argc != 2){
		printf("Usage: %s [port]\n", argv[0]);
		return 1;
	}

	int listen_sock = startup(atoi(argv[1]));

	daemon(0, 0);
	while(1){
		struct sockaddr_in peer;
		bzero(&peer, sizeof(peer));
		socklen_t len = sizeof(peer);
		int sock = accept(listen_sock, (struct sockaddr*)&peer, &len);
		if(sock < 0){
			//write log
			continue;
		}
		int id = fork();
		if(id == 0){ //child
			close(listen_sock);
			if(fork() > 0){
				exit(0);
			}
			service(sock);
			close(sock);
			exit(0);
		}else{
			close(sock);
			waitpid(id, NULL, 0);
		}
	}
}












