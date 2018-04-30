#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void sig_alarm(int signo){
    (void)signo;
}

unsigned int mysleep(unsigned int second){
    struct sigaction New,old;
    New.sa_flags = 0;
    New.sa_handler = sig_alarm;
    sigemptyset(&New.sa_mask);
    sigaction(SIGALRM, &New, &old);
    alarm(second);//这里有BUG
    pause();
    unsigned int unslept = alarm(0);
    sigaction(SIGALRM, &old, NULL);
    return unslept;
}

int main(){
    while(1){
        mysleep(2);
        printf("Hello, But I still have a BUG.\n");
    }
    return 0;
}
