#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    struct sigaction news,old;
    news.sa_handler = SIG_IGN;
    news.sa_flags = 0;
    sigemptyset(&news.sa_mask);
    sigaction(SIGCHLD,&news,&old);
    pid_t cid;
    if((cid = fork()) == 0){
        printf("child:%d\n",getpid());
        sleep(3);
        exit(1);
    }

    while(1){
        printf("father proc is doing something1\n");
        sleep(1);
    }
    return 0;
}
