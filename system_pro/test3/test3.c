#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#define READ 0
#define WRITE 1

int main(){
    int x = 1;
    int i = 0;
    pid_t pid;
    int pipe1[2],pipe2[2];
    //初始化管道
    pipe(pipe1);
    pipe(pipe2);
    pid = fork();

    if(pid < 0){
        //fork出错
        perror("fork");
        exit(1);
    }else if(pid == 0){
        //子进程
        close(pipe1[WRITE]);
        close(pipe2[READ]);
        for(; i<5; i++){
            read(pipe1[READ], &x,sizeof(int));
            printf("child:%d, read: %d\n",getpid(),x++);
            write(pipe2[WRITE], &x, sizeof(int));
        }
        close(pipe2[WRITE]);
        close(pipe1[READ]);
        exit(0);
    }else if(pid > 0){
        //父进程
        close(pipe2[WRITE]);
        close(pipe1[READ]);
        for(; i<5; i++){
            write(pipe1[WRITE], &x, sizeof(int));
            read(pipe2[READ], &x,sizeof(int));
            printf("father:%d, read: %d\n",getpid(),x++);
        }
        close(pipe1[WRITE]);
        close(pipe2[READ]);
    }
    return 0;
}
