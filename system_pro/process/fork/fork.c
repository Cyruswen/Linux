#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


//这是注释
int main(){
    pid_t pid;
    printf("Before: pid is %d\n", getpid());
    pid = fork();
    if(pid == -1){
        perror("fork()");
        exit(1);
    }
    printf("After:pid is %d, fork return %d\n",getpid(),pid);
    sleep(1);
    return 0;
}
