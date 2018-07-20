#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define ERR_EXIT(m) \
    do\
    {\
        perror(m);\
        exit(EXIT_FAILURE);\
    }while(0);

int main()
{
    int pipfd[2];
    if(pipe(pipfd) == -1)
    {
        ERR_EXIT("pipe error");
    }
    pid_t pid;
    pid = fork();
    if(pid == -1)
    {
        ERR_EXIT("fork error");
    }else if(pid == 0)
    {
        close(pipfd[0]);
        write(pipfd[1], "hello", 5);
        close(pipfd[1]);
        exit(EXIT_SUCCESS);
    }else
    {
        close(pipfd[1]);
        char buf[10] = {0};
        read(pipfd[0], buf, 10);
        printf("buf = %s\n", buf);
    }
    return 0;
}
