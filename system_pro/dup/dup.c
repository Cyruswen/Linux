#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    int fd = open("./log", O_CREAT | O_RDWR);
    if(fd < 0)
    {
        perror("open");
        return -1;
    }
    close(1);
    int newfd = dup(fd);
    if(newfd != 1)
    {
        perror("dup");
        return -2;
    }
    close(fd);
    printf("newfd:%d\n", newfd);

    while(1)
    {
        char buf[1024] = {0};
        ssize_t read_size = read(0, buf, sizeof(buf)-1);
        if(read_size < 0)
        {
            perror("read_size");
            continue;
        }
        printf("buf:%s\n", buf);
        fflush(stdout);
    }
    close(newfd);
    return 0;
}
