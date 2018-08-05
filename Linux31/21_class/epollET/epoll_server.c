#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

#define MAX 128

typedef struct epdata{
	int fd;
	int cap;
	int size;
	char buff[0];
}epdata_t, *epdata_p;

void setNonBlock(int fd)
{
	int fl = fcntl(fd, F_GETFL);
	if(fl < 0){
		return;
	}

	fcntl(fd, F_SETFL, fl | O_NONBLOCK);
}

static int startup(int port)
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0){
		perror("socket");
		exit(2);
	}

	int opt = 1;
	setsockopt(sock, SOL_SOCKET,\
			SO_REUSEADDR, &opt, sizeof(opt));

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = htonl(INADDR_ANY);
	local.sin_port = htons(port);

	if(bind(sock, (struct sockaddr*)&local,\
				sizeof(local)) < 0){
		perror("bind");
		exit(3);
	}
	
	if(listen(sock, 5) < 0){
		perror("listen");
		exit(4);
	}
	
	return sock;
}

static void *buyEpdata(int fd, int num)
{
	epdata_p ptr = (epdata_p)malloc(sizeof(epdata_t)\
			+num*sizeof(char));
	if(!ptr){
		perror("malloc");
	}else{
		ptr->fd = fd;
		ptr->cap = num;
		ptr->size = 0;
	}

	return ptr;
}

int my_accept(int listen_sock, int epfd)
{
	int new_fd;
	struct epoll_event ev;
	do{
	    struct sockaddr_in client;
	    socklen_t len = sizeof(client);
	    new_fd = accept(listen_sock,\
	    		(struct sockaddr*)&client,\
	    		&len);
	    if(new_fd < 0){
			break;
	    }

		setNonBlock(new_fd);
	    printf("get a new client[%s:%d]\n", \
	    		inet_ntoa(client.sin_addr),\
	    		ntohs(client.sin_port));

	    ev.events = EPOLLIN|EPOLLET;
	    ev.data.ptr = buyEpdata(new_fd, 1024*5);

	    epoll_ctl(epfd, EPOLL_CTL_ADD,\
	    		new_fd, &ev);
	}while(1);
}

int my_read(int fd, char buf[], int num)
{
	int total = 0;
	ssize_t s;
	while((s = read(fd, buf+total, num-total)) > 0){
		total+=s;
	}
	buf[total] = 0;
	return total;
}

static void serviceIO(int epfd, \
		struct epoll_event *revs,\
		int num,
		int listen_sock)
{
	int i = 0;
	struct epoll_event ev;
	for(; i < num; i++){
		int fd = ((epdata_p)(revs[i].data.ptr))->fd;

		if(revs[i].events & EPOLLIN){
			//read
			if(fd == listen_sock){//accept
				my_accept(fd, epfd);
			}
			else{
				char *buf = ((epdata_p)(revs[i].data.ptr))->buff;
				//int size = ((epdata_p)(revs[i].data.ptr))->size;
				int cap = ((epdata_p)(revs[i].data.ptr))->cap;
	//			ssize_t s = read(fd, buf+size, cap-size-1);
				int total=my_read(fd, buf, cap);
				if(total > 0){
					
					((epdata_p)(revs[i].data.ptr))->size+=total;
					printf("client:> %s\n", buf);
					ev.events = EPOLLOUT;
					ev.data.ptr = revs[i].data.ptr;
					epoll_ctl(epfd, EPOLL_CTL_MOD, fd,\
							&ev);
				}
				else if(total== 0){
					printf("client quit!\n");
					close(fd);
					epoll_ctl(epfd, EPOLL_CTL_DEL,\
							fd, NULL);
					char *buf = (char*)revs[i].data.ptr;
					free(buf);
				}else{
					perror("read");
					close(fd);
					epoll_ctl(epfd, EPOLL_CTL_DEL,\
							fd, NULL);
					char *buf = (char*)revs[i].data.ptr;
					free(buf);
				}
			}
		}
		if(revs[i].events & EPOLLOUT){
			//write
			//char *buf = (char*)revs[i].data.ptr;
			const char *msg = "HTTP/1.0 200 OK\r\n\r\n<html><h1>EPOLL SUCCESS:)</h1></html>\r\n";
			write(fd, msg, strlen(msg));
			close(fd);
			epoll_ctl(epfd, EPOLL_CTL_DEL,fd, NULL);
			char *buf = (char*)revs[i].data.ptr;
			free(buf);
		}
	}
}

int main(int argc, char *argv[])
{
	if(argc != 2){
		printf("Usage: %s [port]\n", argv[0]);
		return 1;
	}
	int listen_sock = startup(atoi(argv[1]));
	setNonBlock(listen_sock);

	int epfd = epoll_create(256);
	if(epfd < 0){
		perror("epoll_create");
		return 5;
	}

	struct epoll_event ev;
	ev.events = EPOLLIN | EPOLLET;
	ev.data.ptr = buyEpdata(listen_sock, 1);
	epoll_ctl(epfd, EPOLL_CTL_ADD, listen_sock, &ev);

	struct epoll_event revs[MAX];
	int size = 0;
	for(;;){
		switch((size=epoll_wait(epfd, revs, MAX,-1))){
			case -1:
				perror("epoll_wait");
				break;
			case 0:
				printf("timeout...\n");
				break;
			default:
				serviceIO(epfd, revs, size, listen_sock);
				break;
		}
	}
}















