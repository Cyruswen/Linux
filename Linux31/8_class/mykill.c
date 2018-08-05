#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

int count = 0;

void handler(int signo)
{
	printf("get a signo : %d, count is :%d\n", signo, count);
	exit(1);
}

int main()
{
	signal(14, handler);
	alarm(1);
	while(1){
		count++;
	//	printf("hello world: %d\n", count++);
	}
}

///void handler(int signo)
//{
//	printf("get a signo: %d\n", signo);
//}
//
////./mykill 9 1234
//int main(int argc, char *argv[])
//{
//	int i = 1;
//	for(; i < 32; i++){
//		signal(i, handler);
//	}
//	abort();
//	for(i=1; i < 32; i++){
//		raise(i);
//		sleep(1);
//	}
//	if(argc != 3){
//		printf("Usage: %s signo proc_id\n", argv[0]);
//		return 1;
//	}
//
//	
//	kill(atoi(argv[2]), atoi(argv[1]));
//}
