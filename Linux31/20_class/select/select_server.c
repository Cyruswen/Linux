#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/select.h>

#define MAX_FD sizeof(fd_set)*8
#define INIT -1

static void array_init(int fd_array[], int num)
{
	int i = 0;
	for(; i < num; i++){
		fd_array[i] = INIT;
	}
}

static int array_add(int fd_array[], int num, int fd)
{
	int i = 0;
	for(; i < num; i++){
		if(fd_array[i] == INIT){
			fd_array[i] = fd;
			return 0;
		}
	}
	return -1;
}

static void array_del(int fd_array[], int num, int index)
{
	if(index < num && index > 0){
		fd_array[index] = INIT;
	}
}

int set_rfds(int fd_array[], int num, fd_set *rfds)
{
	int i = 0;
	int max_fd = INIT;
	printf("select : ");
	for(; i < num; i++){
		if(fd_array[i] > INIT){
			printf(" %d ", fd_array[i]);
			FD_SET(fd_array[i], rfds);

			if(max_fd < fd_array[i]){
				max_fd = fd_array[i];
			}
		}
	}
	printf("\n");

	return max_fd;
}

int service(int fd_array[], int num, fd_set *rfds)
{
	int i = 0;
	for(; i < num; i++){
		if(fd_array[i] > INIT && FD_ISSET(fd_array[i], rfds)){
			int fd = fd_array[i];
			if(i == 0){//listen sock ready
				struct sockaddr_in client;
				socklen_t len = sizeof(client);
				int new_sock = accept(fd, \
						(struct sockaddr*)&client, &len);
				if(new_sock < 0){
					perror("accept");
					continue;
				}
				printf("get a new client, [%s:%d]\n", \
						inet_ntoa(client.sin_addr),\
						ntohs(client.sin_port));
				if(array_add(fd_array, num, new_sock) < 0){
					printf("server is busy!\n");
					close(new_sock);
				}
			}
			else{
				char buf[10240] = {0};
				ssize_t s = read(fd, buf, sizeof(buf)-1);
				if(s > 0){
					buf[s] = 0;
					printf("client:> %s\n", buf);
				}
				else if(s == 0){
					close(fd);
					printf("client quit!\n");
					array_del(fd_array, num, i);
				}
				else{
					perror("read");
					close(fd);
					array_del(fd_array, num, i);
				}
			}
		}
	}
}

int startup(int port)
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0){
		perror("socket");
		exit(2);
	}

	int opt = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(port);
	local.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(sock, (struct sockaddr*)&local, sizeof(local)) < 0){
		perror("bind");
		exit(3);
	}

	if(listen(sock, 5) < 0){
		perror("listen");
		exit(4);
	}

	return sock;
}

// ./select_server 9999
int main(int argc, char *argv[])
{
	if( argc != 2 ){
		printf("Usage: %s [port(>1024)]\n", argv[0]);
		return 1;
	}

	int listen_sock = startup(atoi(argv[1]));
	int fd_array[MAX_FD];

	array_init(fd_array, MAX_FD);
	array_add(fd_array, MAX_FD, listen_sock);

	fd_set rfds;
	int max_fd = 0;
	for(;;){
		struct timeval timeout = {5, 0};
		FD_ZERO(&rfds);
		max_fd = set_rfds(fd_array, MAX_FD, &rfds);
		switch(select(max_fd+1, &rfds, NULL, NULL, NULL/*&timeout)*/)){
			case 0:
				printf("select timeout...!\n");
				break;
			case -1:
				perror("select");
				break;
			default:
				service(fd_array, MAX_FD, &rfds);
				break;
		}
	}
}









