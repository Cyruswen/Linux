#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/syscall.h>

pthread_mutex_t lock;
pthread_cond_t cond;

typedef struct _Node{
	int data;
	struct _Node *next;
}node_t, *node_p, **node_pp;


node_p alloc_node(int x)
{
	node_p p = (node_p)malloc(sizeof(node_t));
	if(!p){
		perror("malloc");
		exit(1);
	}
	p->data = x;
	p->next = NULL;
	return p;
}

int is_empty(node_p h)
{
	return h->next == NULL?1:0;
}

void init_list(node_pp h)
{
	*h = alloc_node(0);
}

void list_push(node_p h, int x)
{
	node_p p = alloc_node(x);
	p->next = h->next;
	h->next = p;
}

void list_pop(node_p head, int *x)
{
	if(!is_empty(head)){
		node_p p = head->next;
		*x = p->data;
		head->next = p->next;
		free(p);
		p=NULL;
	}
}

void list_destroy(node_p h)
{
	int x;
	while(!is_empty(h)){
		list_pop(h, &x);
	}

	free(h);
}

void show_list(node_p h)
{
	if(!is_empty(h)){
		node_p p = h->next;
		while(p){
			printf("%d ", p->data);
			p = p->next;
		}
		printf("\n");
	}
}

void *consumer(void *arg)
{
	node_p head = (node_p)arg;
	while(1){
		int data;

		pthread_mutex_lock(&lock);
		while(is_empty(head)){ //bug?
			printf("no data for consumer!\n");
			pthread_cond_wait(&cond, &lock);
		}
		list_pop(head , &data);
		printf("consumer done! data is : %d\n", data);
		pthread_mutex_unlock(&lock);
	}
}

void *product(void *arg)
{
	node_p head = (node_p)arg;
	while(1){
		int data=0;
		data = rand()%100 + 1;

		pthread_mutex_lock(&lock);
		list_push(head, data);
		pthread_mutex_unlock(&lock);
		pthread_cond_signal(&cond);

		printf("product done! data is : %d\n", data);
		sleep(5);
	}
}

int main()
{
	node_p head;
	init_list(&head);
	pthread_mutex_init(&lock, NULL);
	pthread_cond_init(&cond, NULL);

	srand((unsigned long)time(NULL));

	pthread_t c,p;
	pthread_create(&c, NULL, consumer, (void *)head);
	pthread_create(&p, NULL, product, (void *)head);

	pthread_join(c, NULL);
	pthread_join(p, NULL);
	list_destroy(head);
	pthread_mutex_destroy(&lock);
	pthread_cond_destroy(&cond);


//	int i = 0;
//	for(; i < 10; i++){
//		list_push(head, i);
//		show_list(head);
//		sleep(1);
//	}
//
//	int x;
//	for(i=0; i<5; i++){
//		list_pop(head, &x);
//		show_list(head);
//		sleep(1);
//	}
//
}



//pthread_mutex_t lock;
//
//int ticket = 50000;
//
//void *buy_ticket(void *arg)
//{
//	const char *id = (const char *)arg;
//
//	while(1){
//		pthread_mutex_lock(&lock);
//		if(ticket > 0){
//			//usleep(1000);
//			printf("%s get a ticket: %d\n", id, ticket);
//			ticket--;
//			pthread_mutex_unlock(&lock);
//		}else{
//			pthread_mutex_unlock(&lock);
//			break;
//		}
//	}
//}
//
//int main()
//{
//	pthread_mutex_init(&lock, NULL);
//
//	pthread_t tid1, tid2, tid3, tid4;
//	pthread_create(&tid1, NULL, buy_ticket, "thread_1");
//	pthread_create(&tid2, NULL, buy_ticket, "thread_2");
//	pthread_create(&tid3, NULL, buy_ticket, "thread_3");
//	pthread_create(&tid4, NULL, buy_ticket, "thread_4");
//
//	pthread_join(tid1, NULL);
//	pthread_join(tid2, NULL);
//	pthread_join(tid3, NULL);
//	pthread_join(tid4, NULL);
//
//	pthread_mutex_destroy(&lock);
//}
//
//
////void *thread_run(void *arg)
////{
////	const char *msg = (const char *)arg;
////	printf("%s: tid: %#x, pid: %d\n", msg, pthread_self(), getpid());
////	sleep(2);
////	int a = 10;
////	a/=0;
////}
////
////int main()
////{
////	pthread_t tid1;
////	pthread_create(&tid1, NULL, thread_run, "thead 1");
////	pthread_detach(tid1);
////
////	while(1)
////	{
////		printf("I am main thread!\n");
////		sleep(1);
////	}
////}
////
////
////
////
////
////
////
////
