#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

void handler(int sig){
    pid_t id;
    while((id = waitpid(-1,NULL,WNOHANG)) > 0){
        printf("wait child success:%d\n",id);
    }
    printf("child is quit!%d\n",getpid());
}
int main(){
    signal(SIGCHLD,handler);
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
