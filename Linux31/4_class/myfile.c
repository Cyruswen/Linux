
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main()
{
	const char *msg1 = "hello printf\n";
	const char *msg2 = "hello fwrite\n";
	const char *msg3 = "hello write\n";

	printf(msg1);
	fwrite(msg2, 1, strlen(msg2), stdout);
	write(1, msg3, strlen(msg3));

	//fork();

	//close(1);
	//int fd = open("file", O_WRONLY|O_CREAT, 0644);
	//if(fd < 0){
	//	perror("open");
	//	return 1;
	//}

	//int count = 10;
	//while(count--){
	//	printf("hello wrold\n");
	//	fflush(stdout);
	//}

	//close(fd);
//	//int fd1 = open("file1", O_RDONLY, 0644);
//	int fd2 = open("file2", O_RDONLY, 0644);
//	int fd3 = open("file3", O_RDONLY, 0644);
//
//	printf("fd: %d\n", fd);
//	printf("fd: %d\n", fd1);
//	printf("fd: %d\n", fd2);
//	printf("fd: %d\n", fd3);
//
//	const char *msg = "hello world\n";
//	int size = strlen(msg);
//	char buf[64];
//	int count = 5;
//	while(1){
//		size_t s = read(0, buf, size);
//		if(s > 0){
//			buf[s] = 0;
//		}else if(s == 0){
//			break;
//		}
//		printf("%s", buf);
//	}

}






