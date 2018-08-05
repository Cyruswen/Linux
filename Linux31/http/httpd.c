#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <pthread.h>

#define MAX 1024
#define HOME_PAGE "index.html"

static void usage(const char *proc)
{
	printf("Usage: %s port\n", proc);
}

static int startup(int port)
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock<0){
		perror("socket");
		exit(2);
	}

	int opt = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = htonl(INADDR_ANY);
	local.sin_port = htons(port);

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

int get_line(int sock, char line[], int size)
{
	int c = 'a';
	int i = 0;
	ssize_t s = 0;
	while(i < size-1 && c != '\n'){
		s = recv(sock, &c, 1, 0);
		if(s > 0){
			if(c == '\r'){
				//\r->\n or \r\n -> \n
				recv(sock, &c, 1, MSG_PEEK); //? -> \r
				if(c != '\n'){
					c = '\n';
				}
				else{
					recv(sock, &c, 1, 0);
				}
			}
			//c == \n
			line[i++] = c; //\n
		}
		else{
			break;
		}
	}
	line[i] = '\0';
	return i;
}

void clear_header(int sock)
{
	char line[MAX];
	do{
		get_line(sock, line, sizeof(line));
	}while(strcmp(line, "\n") != 0);
}

void echo_www(int sock, char *path, int size, int *err)
{
	clear_header(sock);

	int fd = open(path, O_RDONLY);
	if(fd < 0){
		*err = 404;
		return;
	}
	char line[MAX];
	sprintf(line, "HTTP/1.0 200 OK\r\n");
	send(sock, line, strlen(line), 0);
	sprintf(line, "Content-Type: text/html\r\n");
	send(sock, line, strlen(line), 0);
	//sprintf(line, "Content-Type: image/jpg\r\n");
	//send(sock, line, strlen(line), 0);
	sprintf(line, "\r\n");
	send(sock, line, strlen(line), 0);
	sendfile(sock, fd, NULL, size);
	close(fd);
}

void show_404(int sock)
{
	char line[MAX];
	sprintf(line, "HTTP/1.0 404 Not Found\r\n");
	send(sock, line, strlen(line), 0);
	sprintf(line, "Content-Type: text/html\r\n");
	send(sock, line, strlen(line), 0);
	sprintf(line, "\r\n");
	send(sock, line, strlen(line), 0);

	const char *err = "wwwroot/404.html";
	int fd = open(err, O_RDONLY);
	struct stat st;
	stat(err, &st);
	sendfile(sock, fd, NULL, st.st_size);
	close(fd);
}

void echo_error(int sock, int code)
{
	clear_header(sock);
	switch(code){
		case 403:
			break;
		case 404:
			show_404(sock);
			break;
		case 501:
			break;
		case 503:
			break;
		default:
			break;
	}
}
			

int exe_cgi(int sock, char path[], char method[], char *query_string)
{
	char line[MAX];
	int content_length = -1;

	char method_env[MAX/32];
	char query_string_env[MAX];
	char content_length_env[MAX/16];

	if(strcasecmp(method, "GET") == 0){
		clear_header(sock);
	}else{ //POST
	    do{
	    	get_line(sock, line, sizeof(line));
			if(strncmp(line, "Content-Length: ", 16) == 0){
				content_length = atoi(line+16);
			}
	    }while(strcmp(line, "\n") != 0);
		if(content_length == -1){
			return 404;
		}
	}

	sprintf(line, "HTTP/1.0 200 OK\r\n");
	send(sock, line, strlen(line), 0);
	sprintf(line, "Content-Type: text/html\r\n");
	send(sock, line, strlen(line), 0);
	sprintf(line, "\r\n");
	send(sock, line, strlen(line), 0);

	int input[2];
	int output[2];

	pipe(input);
	pipe(output);

	pid_t id = fork();
	if(id < 0){
		return 404;
	}
	else if(id == 0){//child
		close(input[1]); 
		close(output[0]);
		dup2(input[0], 0);
		dup2(output[1], 1);

		sprintf(method_env, "METHOD=%s", method);
		putenv(method_env);
		if(strcasecmp(method, "GET")==0){
			sprintf(query_string_env, "QUERY_STRING=%s", query_string);
			putenv(query_string_env);
		}
		else{
			sprintf(content_length_env, "CONTENT_LENGTH=%d",\
					content_length);
			putenv(content_length_env);
		}

		printf("method: %s");
		// method, GET[query_string], POST[content_length]
		execl(path, path, NULL);
		exit(1);
	}
	else{ //father
		close(input[0]);
		close(output[1]);

		char c;
		if(strcasecmp(method, "POST") == 0){
			int i = 0;
			for(;i < content_length; i++){
				read(sock, &c, 1);
				write(input[1], &c, 1);
			}
		}

		while(read(output[0], &c, 1) > 0){
			send(sock, &c, 1, 0);
		}

		waitpid(id, NULL, 0);
		close(input[1]);
		close(output[0]);
	}
	return 200;
}

static void *handler_request(void *arg)
{
	int sock = (int)arg;
	char line[MAX];
	char method[MAX/32];
	char url[MAX];
	char path[MAX];
	int errCode = 200;
	int cgi = 0;
	char *query_string = NULL;

#ifdef Debug
	do{
		get_line(sock, line, sizeof(line));
		printf("%s", line);
	}while(strcmp(line, "\n") != 0);
#else

	if(get_line(sock, line, sizeof(line)) < 0){
		errCode = 404;
		goto end;
	}

	//get method
	//line[] = GET /index.php?a=100&&b=200 HTTP/1.1
	int i = 0;
	int j = 0;
	while(i < sizeof(method)-1 && j < sizeof(line) && !isspace(line[j])){
		method[i] = line[j];
		i++, j++;
	}
	method[i] = '\0';

	//gEt, post, Post, pOst
	if(strcasecmp(method, "GET") == 0){
	}
	else if(strcasecmp(method, "POST") == 0){
		cgi = 1;
	}
	else{
		errCode = 404;
		goto end;
	}

	while(j < sizeof(line) && isspace(line[j])){
		j++;
	}
	i=0;
	while(i < sizeof(url)-1 && j < sizeof(line) && !isspace(line[j])){
		url[i] = line[j];
		i++, j++;
	}
	url[i] = '\0';

	//url
	if(strcasecmp(method, "GET")==0){
		query_string = url;
		while(*query_string){
			if(*query_string == '?'){
				*query_string = '\0';
				query_string++;
				cgi = 1;
				break;
			}
			query_string++;
		}
	}

	// url -> wwwroot/a/b/c.html| url -> wwwroot/
	sprintf(path, "wwwroot%s", url);
	if(path[strlen(path)-1] == '/'){
		strcat(path, HOME_PAGE);
	}

	struct stat st;
	if(stat(path, &st) < 0){
		errCode = 404;
		goto end;
	}
	else{
		if(S_ISDIR(st.st_mode)){
			strcat(path, HOME_PAGE);
		}else{
		    if((st.st_mode & S_IXUSR) ||\
		    		(st.st_mode & S_IXGRP) ||\
		    		(st.st_mode & S_IXOTH)){
		    	cgi = 1;
		    }
		}
		if(cgi){
			errCode=exe_cgi(sock, path, method, query_string);
		}
		else{
			printf("method: %s,path: %s\n",method,path);
			// method[GET, POST], cgi[0|1], url[], query_String[NULL|arg]
			echo_www(sock, path, st.st_size, &errCode);
		}
	}
	
#endif
end:
	if(errCode != 200){
		echo_error(sock, errCode);
	}
	close(sock);
}

//httpd 8080
int main(int argc, char *argv[])
{
	if(argc != 2){
		usage(argv[0]);
		return 1;
	}
	int listen_sock = startup(atoi(argv[1]));
	//daemon(0, 0);
	signal(SIGPIPE, SIG_IGN);
	for(;;){
		struct sockaddr_in client;
		socklen_t len = sizeof(client);
		int new_sock = accept(listen_sock,\
				(struct sockaddr*)&client, &len);
		if(new_sock < 0)
		{
			perror("accept");
			continue;
		}
		pthread_t id;
		pthread_create(&id, NULL, handler_request, (void *)new_sock);
		pthread_detach(id);
	}
}









