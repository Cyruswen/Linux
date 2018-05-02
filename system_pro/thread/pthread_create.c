#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

void *rout(void *arg){
    (void)arg;
    while(1){
        printf("I am thread 1\n");
        sleep(1);
    }
}

int main(void){
    pthread_t tid;
    int ret;
    if((ret = pthread_create(&tid, NULL, rout, NULL)) != 0){
        fprintf(stderr, "pthread_create:%s\n", strerror(ret));
        exit(EXIT_FAILURE);
    }

    while(1){
        printf("I am main thread\n");
        sleep(1);
    }
}
