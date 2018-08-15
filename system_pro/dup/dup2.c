#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    int fd = open("./log2",O_CREAT | O_RDWR);
    if(fd < 0)
    {
        perror("open");
        return -1;
    }
    close(1);
    dup2(fd, 1);
    close(fd);
    while(1)
    {
        char buf[1024] = {0};
        ssize_t size = read(0, buf, sizeof(buf)-1);
        if(size < 0)
        {
            perror("read");
            break;
        }
        printf("buf:%s\n", buf);
        fflush(stdout);
    }
    return 0;
}
