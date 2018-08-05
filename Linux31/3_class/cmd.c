
#include <stdio.h>

int main()
{
	int i = 0;
	for(; i < 10; i++){
		printf("hello world! %d, %s\n", i, getenv("MYENV"));
	}
}
