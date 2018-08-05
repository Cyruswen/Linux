
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include <pthread.h>

#define M 32

sem_t semBlank;
sem_t semData;

void *consumer(void *arg)
{
	int *ring=(int*)arg;
	int i=0;
	while(1){
		//P(semData)
		sem_wait(&semData);
		int data = ring[i];
		i++;
		i %= M;
		printf("consumer thread : %x, consumer data is : %d\n", pthread_self(),data);
		sem_post(&semBlank);
		//V(semBlank)
		usleep(5000);
	}
}

void *producter(void *arg)
{
	int *ring=(int*)arg;
	srand((unsigned long)time(NULL));
	int i=0;
	while(1){
		//P(semBlank)
		sem_wait(&semBlank);
		int data = rand()%100+1;
		ring[i] = data;
		printf("product thread : %x, product data is : %d\n", pthread_self(),data);
		i++;
		i %= M;
		sem_post(&semData);
		//V(semData)
		usleep(1000000);
	}
}


int main()
{
	pthread_t c, p;
	int ring[M];

	sem_init(&semBlank, 0, M);
	sem_init(&semData, 0, 0);

	pthread_create(&c, NULL, consumer, (void *)ring);
	pthread_create(&p, NULL, producter, (void *)ring);

	pthread_join(c, NULL);
	pthread_join(p, NULL);

	sem_destroy(&semBlank);
	sem_destroy(&semData);
}




