
#include "comm.h"


typedef struct {
	int sock;
	char ip[24];
	int port;
}net_info_t;

int startup(char *ip, int port)
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0){
		printf("socket error!\n");
		exit(2);
	}

	int opt = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

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

void service(int sock, char *ip, int port)
{
//	char buf[10240];
//
//	read(sock, buf, sizeof(buf));
//
//	printf("%s", buf);
//	printf("###################################\n");
//	request_t r;
//
//	read(sock, &r, sizeof(r));
//	
//	response_t rp;
//	rp.res = r.x + r.y;
//
//	write(sock, &rp, sizeof(rp));
//
	char buf[MAX];
	while(1){
		buf[0] = 0;
		ssize_t s = read(sock, buf, sizeof(buf)-1);
		if(s > 0){
			buf[s] = 0;
			printf("[%s:%d] say# %s\n", ip, port, buf);
			write(sock, buf, strlen(buf));
		}
		else if(s == 0){
			printf("client [%s:%d] quit!\n", ip, port);
			break;
		}else{
			printf("read error!\n");
			break;
		}
	}
}

void *thread_service(void *arg)
{
	net_info_t *p = (net_info_t*)arg;
	service(p->sock, p->ip, p->port);

	close(p->sock);
	free(p);
}

//./tcpServer 127.0.0.1 8080
int main(int argc, char *argv[])
{
	if(argc != 3){
		printf("Usage: %s [ip] [port]\n", argv[0]);
		return 1;
	}

	int listen_sock = startup(argv[1], atoi(argv[2]));

	struct sockaddr_in peer;
	char ipBuf[24];
	for(;;){
		ipBuf[0] = 0;
		socklen_t len = sizeof(peer);
		int new_sock = accept(listen_sock, (struct sockaddr*)&peer, &len);
		if(new_sock < 0){
			printf("accept error!\n");
			continue;
		}
		inet_ntop(AF_INET, (const void *)&peer.sin_addr,\
				ipBuf, sizeof(ipBuf));
		int p = ntohs(peer.sin_port);
		printf("get a new connect, [%s:%d]\n",\
				ipBuf, p);

		net_info_t *info = (net_info_t*)malloc(sizeof(net_info_t));
		if(info == NULL){
			perror("malloc");
			close(new_sock);
			continue;
		}
		info->sock = new_sock;
		strcpy(info->ip, ipBuf);
		info->port = p;

		pthread_t tid;
		pthread_create(&tid, NULL, thread_service, (void *)info);
		pthread_detach(tid);

	//	pid_t id = fork();
	//	if(id == 0){//child
	//		close(listen_sock);

	//		if(fork() > 0){
	//			exit(0);
	//		}

	//		service(new_sock, ipBuf, p);
	//		close(new_sock);
	//		exit(0);
	//	}
	//	else if(id > 0){//father
	//		close(new_sock);
	//		waitpid(id, NULL, 0);
	//	}
	//	else{
	//		printf("fork error!\n");
	//		continue;
	//	}
	}

	return 0;
}












