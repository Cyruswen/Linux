#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int main()
{
	signal(SIGCHLD, SIG_IGN);
	if(fork() == 0){
		int i = 5;
		while(i){
			printf("I am child, pid: %d, ppid: %d\n", getpid(),getppid());
			sleep(1);
			i--;
		}
		exit(1);
	}

	while(1);
}

//void handler(int signo)
//{
//	pid_t ret;
//	while((ret = waitpid(-1, NULL, WNOHANG)) > 0){
//		if(ret > 0){
//			printf("wait success, child pid: %d\n", ret);
//		}
//	}	
//}
//
//int main()
//{
//	signal(SIGCHLD, handler);
//	if(fork() == 0){//child
//		int i = 0;
//		while(i < 5){
//			printf("I am child, pid: %d, ppid: %d\n", getpid(),getppid());
//			sleep(1);
//			i++;
//		}
//		exit(1);
//	}
//
//	while(1);
//
//}
//
////void handler(int signo)
////{
////
////}
////
////int main()
////{
////	signal(2, handler);
////
////	int ret = pause();
////	printf("%d\n", ret);
////}
////
//////void handler(int sig)
//////{
//////	printf("get a sig: %d\n", sig);
//////}
//////
//////int main()
//////{
//////	struct sigaction act, oact;
//////	act.sa_handler = handler;
//////	act.sa_flags = 0;
//////	sigemptyset(&act.sa_mask);
//////	sigaction(2, &act, &oact);
//////
//////	while(1);
//////
//////
//////	return 0;
//////}
//////
//////
////////void showpending(sigset_t *p)
////////{
////////	int i = 1;
////////	for(; i < 32; i++){
////////		if(sigismember(p, i)){
////////			printf("1 ");
////////		}else{
////////			printf("0 ");
////////		}
////////	}
////////	printf("\n");
////////}
////////
////////void handler(int signo)
////////{
////////	printf("get a sig: %d\n", signo);
////////}
////////
////////int main()
////////{
////////	signal(SIGINT, handler);
////////	sigset_t set, oset;
////////	sigset_t pending;
////////	sigemptyset(&set);
////////	sigaddset(&set, 2);
////////
////////	sigprocmask(SIG_SETMASK, &set, &oset);
////////	int i = 0;
////////	while(1){
////////		sigpending(&pending);
////////		showpending(&pending);
////////		sleep(1);
////////		if(i==10){
////////			sigprocmask(SIG_SETMASK, &oset, NULL);
////////			printf("revover block bitmap!\n");
////////		}
////////		i++;
////////	}
////////
////////	return 0;
////////}
////////
////////
////////
////////
////////
////////
////////
////////
////////
