
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int main()
{

	int fd = open("myfile.txt", O_CREAT | O_WRONLY, 0666);
	if(fd < 0){
		return 1;
	}

	printf("fd: %d\n", fd);

	const char *msg = "hello world!\n";
	write(0, msg, strlen(msg));

	close(fd);


	//FILE *fp = fopen("myfile.txt", "w");
	//if(!fp){
	//	perror("fopen");
	//	return 1;
	//}

	//int i = 0;
	//const char *msg = "hello bit!\n";
	//while(i < 10){
	//	fwrite(msg, 1, strlen(msg), fp);
	//	i++;
	//}

	//fclose(fp);
}
