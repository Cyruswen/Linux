
#include <stdio.h>
#include <pthread.h>

int counter=0;

pthread_rwlock_t rwlock;

void *route_write(void *arg)
{
	int number = (int)arg;
	int tmp = 0;
	while(1){
		pthread_rwlock_wrlock(&rwlock);
		tmp = counter;
		printf("thread %d, tid: %x, old: %d, new: %d\n",\
				number, pthread_self(), tmp, ++counter);
		pthread_rwlock_unlock(&rwlock);
		usleep(5000);
	}
}

void *route_read(void *arg)
{
	int number = (int)arg;
	while(1){
		pthread_rwlock_rdlock(&rwlock);
		printf("thread %d, tid: %x, current data is : %d\n", \
				number, pthread_self(), counter);
		pthread_rwlock_unlock(&rwlock);
		usleep(1);
	}
}

int main()
{
	pthread_rwlock_init(&rwlock, NULL);
	int i = 0;
	pthread_t t[8];
	for(; i < 3; i++){
		pthread_create(t+i, NULL, route_write, (void *)i);
	}

	for(; i < 8; i++){
		pthread_create(t+i, NULL, route_read, (void *)i);
	}

	for(i=0; i < 8; i++){
		pthread_join(t[i], NULL);
	}
	pthread_rwlock_destroy(&rwlock);
}








