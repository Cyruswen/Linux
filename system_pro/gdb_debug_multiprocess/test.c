#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void processA(){
    printf("I am the child process,%d!\n",getpid());
    return;
}

void processB(){
    printf("I am the parent process,%d!\n",getpid());
}

//这是注释
int main(){
    pid_t pid;
    pid = fork();
    if(pid < 0){
        perror("fork");
        exit(-1);
    }else if(pid == 0){
        processA();
        exit(0);
    }else{
        processB();
        sleep(1);
    }
    return 0;
}
