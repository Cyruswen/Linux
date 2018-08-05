#include <stdio.h>
#include <signal.h>

void wakeup(int signo)
{
}

int mysleep(int t)
{
	//bug!!!
	struct sigaction act, oact;
	act.sa_handler = wakeup;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);
	sigaction(SIGALRM, &act, &oact);
	alarm(t);
	pause();
	int ret = alarm(0);
	sigaction(SIGALRM, &oact, NULL);

	return ret;
}

int main()
{
	while(1){
		printf("hello world!\n");
		mysleep(1);
	}
}







