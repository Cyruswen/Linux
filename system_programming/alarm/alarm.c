#include <stdio.h>
#include <unistd.h>

int main(){
    int count = 14;
    alarm(1);
    for(;1;count++){
        printf("count is %d\r",count);
        fflush(stdout);
    }
    return 0;
}
