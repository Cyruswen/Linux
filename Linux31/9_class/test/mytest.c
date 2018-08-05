#include <stdio.h>

volatile int flag = 0;

void handler(int sig)
{
	flag = 1;
	printf("get a sig: %d\n", sig);
}

int main()
{
	signal(2, handler);
	while(!flag);

	printf("I am quit!\n");
	return 0;
}
