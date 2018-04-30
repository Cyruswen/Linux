#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void sig_alarm(int signo){
    //DO NOTHING
    (void)signo;
}

unsigned int mysleep(unsigned int nsess){
    struct sigaction news;
    struct sigaction old;
    unsigned int unslept = 0;
    news.sa_handler = sig_alarm;
    sigemptyset(&news.sa_mask);
    news.sa_flags = 0;
    sigaction(SIGALRM, &news, &old);
    alarm(nsess);//此处有BUG
    pause();
    unslept = alarm(0);
    sigaction(SIGALRM, &old, NULL);
    return unslept;
}

int main(){
    while(1){
       unsigned int ret =  mysleep(5);
        printf("5 seconds havepassed,%d\n",ret);
    }
    return 0;
}
