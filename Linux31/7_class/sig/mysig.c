

#include <stdio.h>
#include <signal.h>

void handler(int sig)
{
	printf("get a sig, handler %d sig!\n", sig);
}

int main()
{
	signal(2, handler);
	signal(3, handler);
	signal(4, handler);
	while(1)
	{
		int a = 1;
		a/=0;
		printf("I am running!...\n");
		sleep(1);
	}
	return 0;
}
