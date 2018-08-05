
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
	//execl("/bin/ls", "ls", "--color=auto","-a", "-l", "-n", NULL);
	//execlp("ls", "ls", "-a", "-l","--color=auto", NULL);
	char *argv[]={
		"cmd",
		NULL,
	};
	char *env[]={
		"MYENV=100",
		NULL,
	};
	//execv("/usr/bin/top", argv);
	execve("/homA/hb/bit-code/31_class/Linux/3_class/cmd", argv, env);
	exit(1);
}

//int fun()
//{
//	exit(123);
//}
//
//int main()
//{
//	pid_t id = fork();
//	if(id == 0){//child
//		int count = 5000;
//		while(count--){
//			printf("child, pid: %d, ppid: %d\n", getpid(), getppid());
//			sleep(5);
//		}
//		printf("child quit!\n");
//		exit(13);
//	}else{ //father
//		printf("father, pid: %d, ppid: %d\n", getpid(), getppid());
//		//pid_t ret = wait(NULL);
//		int status;
//		while(1){
//	    	pid_t ret = waitpid(id, &status, WNOHANG);
//	    	if(ret > 0){
//	    		printf("sig: %d, exit code: %d\n",\
//	    				status&0x7F, (status>>8)&0xFF);
//				break;
//	    	}else if(ret == 0){
//				printf("child is running! I do other thing!!\n");
//			}else{
//				break;
//			}
//			sleep(1);
//		}
//		printf("father quit!\n");
//		sleep(1);
//	}
//	//while(1){
//	//printf("hello world");
//	//}
//	//sleep(3);
//	//_exit(1);
//}
