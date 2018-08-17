#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Init(int* fd_list, int fd_list_size)
{
    for(int i = 0; i < fd_list_size; i++)
    {
        fd_list[i] = -1;
    }
}

void Reload(int listen_fd, int* connect_list, int connect_list_size, fd_set* read_fds, int* max_fd)
{
    FD_ZERO(read_fds);
    FD_SET(listen_fd, read_fds);
    int max = listen_fd;
    for(int i = 0; i < connect_list_size; i++)
    {
        if(connect_list[i] != -1)
        {
            FD_SET(connect_list[i], read_fds);
            if(connect_list[i] > max)
            {
                max = connect_list[i];
            }
        }
        *max_fd = max;
    }
}

void Add(int fd, int* connect_list, int connect_list_size)
{
    for(int i = 0; i < connect_list_size; i++)
    {
        if(connect_list[i] == -1)
        {
            connect_list[i] = fd;
            break;
        }
    }
}

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("usage:./server [ip] [port]\n");
        return 0;
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));

    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_fd < 0)
    {
        perror("socket");
        return -1;
    }

    int ret = bind(listen_fd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret < 0)
    {
        perror("bind");
        return -1;
    }

    ret = listen(listen_fd, 10);
    if(ret < 0)
    {
        perror("listen");
        return -1;
    }

    fd_set read_fds;
    int fd_list[1024];
    Init(fd_list, sizeof(fd_list)/sizeof(int));
    for(;;){
        int max_fd = listen_fd;
        Reload(listen_fd, fd_list, sizeof(fd_list)/sizeof(int), &read_fds, &max_fd);
        printf("before select:%d\n", FD_ISSET(listen_fd, &read_fds));
        int ret = select(max_fd+1, &read_fds, NULL, NULL, NULL);
        if(ret < 0)
        {
            perror("select");
            continue;
        }

        if(ret == 0)
        {
            printf("select timeout\n");
            continue;
        }

        //listen_fd
        if(FD_ISSET(listen_fd, &read_fds))
        {
            struct sockaddr_in client_addr;
            socklen_t len = sizeof(client_addr);
            int connect_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &len);
            if(connect_fd < 0)
            {
                perror("accept");
                continue;
            }
            printf("client %s:%d connect\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
            Add(connect_fd, fd_list, sizeof(fd_list)/sizeof(int));
        }
        //处理connect_fd
        for(size_t i = 0; i < sizeof(fd_list)/sizeof(int); i++)
        {
            if(fd_list[i] == -1)
            {
                continue;
            }

            if(!FD_ISSET(fd_list[i], &read_fds))
            {
                continue;
            }

            char buf[1024] = {0};
            ssize_t read_size = read(fd_list[i], buf, sizeof(buf)-1);
            if(read_size < 0)
            {
                perror("read");
                close(fd_list[i]);
                fd_list[i] = -1;
                return 2;
            }

            if(read_size == 0)
            {
                printf("client say goodbye!\n");
                close(fd_list[i]);
                fd_list[i] = -1;
                return 2;
            }
            printf("client say: %s\n", buf);
            write(fd_list[i], buf, sizeof(buf)-1);
        }
        
    }
    return 0;
}
