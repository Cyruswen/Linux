
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>

int main()
{
	int fd = open("./fifo", O_WRONLY);//read
	if(fd < 0){
		perror("open");
		return 2;
	}

	char buf[64];
	while(1){
		printf("Please Enter: ");
		scanf("%s", buf);
		if(strcmp(buf, "quit") == 0){
			break;
		}
		write(fd, buf, strlen(buf));
	}

	close(fd);
	return 0;
}










