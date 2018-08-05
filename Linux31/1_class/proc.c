

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
	while(1);
//	pid_t id = fork();
//	if(id == 0){//child
//		while(1)
//		{
//			printf("child do thing 1...,pid: %d, ppid: %d\n", \
//					getpid(), getppid());
//			sleep(1);
//		}
//	}else if(id > 0){//father
//		printf("father do thing 2...,pid: %d, ppid: %d\n", \
//				getpid(), getppid());
//		sleep(5);
//		exit(1);
//	}else{
//		perror("fork");
//	}
	return 0;
}
