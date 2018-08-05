#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>

void mydaemon()
{
	umask(0);
	if(fork() > 0){
		exit(0);
	}

	setsid();
	signal(SIGCHLD, SIG_IGN);

//	chdir("/");

	close(0);
//	close(1);
//	close(2);
}

int main()
{
   // daemon(0, 0);
	//mydaemon();
	while(1)
	{
		//do some service
		sleep(1);
	}
}







