#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int glob = 100;
int main(){
    pid_t pid;
    if(( pid = vfork() ) == -1) perror("vfork"),exit(1);
    if(pid == 0){
        sleep(5);
        glob = 200;
        printf("child glob %d\n", glob);
        exit(0);
    }
    else{
        printf("parent glob %d\n", glob);
    }

    return 0;

}
