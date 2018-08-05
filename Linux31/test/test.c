#include <stdio.h>
#include <unistd.h>


int main()
{
	char path[]="/bin/ls";
	execl(path, path, NULL);
}
