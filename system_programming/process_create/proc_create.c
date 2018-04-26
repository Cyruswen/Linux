#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

typedef void (*FUNC)(char*) ;

void func(char* arg){
    printf("%s,child pid is %d,father pid is %d\n",arg,getpid(),getppid());
    sleep(5);
    return;
}

void process_create(pid_t* pid, void* func, char* arg){
    *pid = fork();
    pid_t id;
    if( *pid < 0 ){
        perror("fork");
        return;
    }else if( *pid == 0 ){
        //child
        FUNC funct = (FUNC)func;
        (*funct)(arg);
        exit(0);
    }else{
        //father
        id = waitpid(-1,NULL,0);
        printf("wait child successful!,child id is %d\n",id);
        return;
    }
    return;
}

int main(){
    pid_t pid;
    process_create(&pid, func, "haha");
    return 0;
}
