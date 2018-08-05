

#include <stdio.h>
#include <stdlib.h>

int g_val = 100;

int main()
{
	printf("hello world!\n");
	return 100;
//	pid_t id = vfork();
//	if(id == 0){//child
//		printf("before: c, pid:%d, ppid:%d,g_val:%d,&g_val:%p\n",\
//				getpid(), getppid(), g_val, &g_val);
//		//g_val = 200;
//		//printf("after: c, pid:%d, ppid:%d,g_val:%d,&g_val:%p\n",\
//				getpid(), getppid(), g_val, &g_val);
//		return 0;
//		//exit(1);
//	}else if(id > 0){//father
//		printf("f, pid:%d, ppid:%d,g_val:%d,&g_val:%p\n",\
//				getpid(), getppid(), g_val, &g_val);
//	}
	//extern char **environ;
    //int i = 0;
	//for(; environ[i]; i++){ // *(environ + i)
	//	printf("%s\n", environ[i]);
	//}
	return 0;
}
