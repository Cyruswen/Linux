
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void *thread_run(void *arg)
{
	printf("I am %s, id is : %lu\n", (char*)arg, \
			pthread_self());
	//sleep(3);
	pthread_exit((void *)99);
}

int main()
{
	pthread_t tid;
	pthread_create(&tid, NULL, thread_run, "thread 1");

	sleep(2);
	pthread_cancel(tid);
	void *ret;
	pthread_join(tid, &ret);

	printf("main thread run new thread ret: %d\n",\
			(int)ret);
	return 21;
}






