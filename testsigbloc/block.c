#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void printsigset(sigset_t* set){
    int i = 0;
    for(; i < 32; i++){
        if(sigismember(set, i)){  //判断指定信号集是否在信号集中
            putchar('1');
        }else{
            putchar('0');
        }
    }
    puts("");
}

int main(){
    sigset_t s,p;
    sigemptyset(&s);  //将信号集置0
    sigaddset(&s,SIGINT);  //将SIGINT添加到信号集s
    sigprocmask(SIG_BLOCK,&s,NULL); //将信号集s设为阻塞信号集
    while(1){
        sigpending(&p);  //获取未决信号集存到p
        printsigset(&p); //打印信号集p
        sleep(1);
    }
    return 0;
}
