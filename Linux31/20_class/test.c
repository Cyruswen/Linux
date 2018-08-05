
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main()
{
	int fd = open("./log.txt", O_WRONLY|O_CREAT, 0644);
	if(fd < 0){
		return 1;
	}

	dup2(fd, 1);

//	close(1);
//	int new_fd = dup(fd);

	printf("hello world!\n");
	printf("hello world!\n");
	printf("hello world!\n");
	printf("hello world!\n");
	fflush(stdout);

	close(fd);
}

//void setNonBlock(int fd)
//{
//	int fl = fcntl(fd, F_GETFL);
//	if(fl < 0){
//		return;
//	}
//
//	fcntl(fd, F_SETFL, fl | O_NONBLOCK);
//}
//
//int main()
//{
//	char buf[64];
////	setNonBlock(0);
//	while(1){
//		ssize_t s = read(0, buf, sizeof(buf)-1);
//		if(s > 0){
//			buf[s] = 0;
//			printf("echo# %s\n", buf);
//		}
//		else if(s == -1 && errno == EAGAIN){
//			printf("stdin data is not ready!\n");
//			sleep(1);
//		}else{
//			printf("error!: %d, %d\n", s, errno);
//		}
//	}
//	return 0;
//}
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
