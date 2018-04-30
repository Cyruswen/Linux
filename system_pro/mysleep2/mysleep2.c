#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void sig_alrm(int signo){
    (void)signo;
}

unsigned int mysleep(unsigned int nsecs){
    struct sigaction newact, oldact;
    sigset_t newmask, oldmask, suspmask;
    
    newact.sa_handler = sig_alrm;
    newact.sa_flags = 0;
    sigemptyset(&newact.sa_mask);;
    sigaction(SIGALRM, &newact, &oldact);

    sigemptyset(&newmask);
    sigaddset(&newmask, SIGALRM);
    sigprocmask(SIG_BLOCK, &newmask, &oldmask);
    alarm(nsecs);
    suspmask = oldmask;
    sigdelset(&suspmask, SIGALRM);
    sigsuspend(&suspmask);
    unsigned int unslept = alarm(0);
    sigaction(SIGALRM, &oldact, NULL);
    sigprocmask(SIG_SETMASK, &oldmask, NULL);
    return unslept;
}

int main(){
    while(1){
        mysleep(2);
        printf("Hello, I don't have BUG now.\n");
    }
    return 0;
}
