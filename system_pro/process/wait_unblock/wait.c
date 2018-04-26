#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    pid_t pid;
    pid = fork();
    if(pid < 0){
        printf("%s fork is failed!\n",__FUNCTION__);
        return 1;
    }else if(pid == 0){
        printf("child is runninig, pid is %d\n",getpid());
        sleep(5);
        exit(1);
    }else{
        int status = 0;
        pid_t ret = 0;
        do{
            ret = waitpid(-1, &status, WNOHANG);//非阻塞式等待
            if(ret == 0){
                printf("child is running\n");
            }
            sleep(1);
        }while(ret == 0);
        
        if(WIFEXITED(status) && ret == pid){
            printf("wait child 5s is success,child return code is:%d.\n",WEXITSTATUS(status));
        }else{
            printf("wait child failed,return.\n");
            return 1;
        }
    }
    return 0;
}
