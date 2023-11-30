#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "util.h"

#define MAX_EVENT 1024

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(sockfd == -1, "sockfd create failed!");

    int option = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(int));
    // 设置非阻塞socket
    fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL) | O_NONBLOCK);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    errif(bind(sockfd, (sockaddr *)&addr, sizeof(addr)) == -1, "bind error");
    errif(listen(sockfd, 5) == -1, "listen error");

    int epfd = epoll_create(5);
    struct epoll_event ev, events[MAX_EVENT];
    ev.data.fd = sockfd;
    ev.events = EPOLLIN | EPOLLET;
    epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);

    while (1)
    {
        int nfds = epoll_wait(epfd, events, MAX_EVENT, -1);
        errif(nfds == -1, "epoll_wait error");

        for (int i = 0; i < nfds; ++i)
        {
            if (events[i].data.fd == sockfd)
            {
                struct sockaddr_in clnt_addr;
                socklen_t clnt_len = sizeof(clnt_addr);
                int clntfd = accept(sockfd, (sockaddr *)&clnt_addr, &clnt_len);
                errif(clntfd == -1, "clnt_addr failed");
                // 设置非阻塞socket
                fcntl(clntfd, F_SETFL, fcntl(clntfd, F_GETFL) | O_NONBLOCK);
                ev.data.fd = clntfd;
                ev.events = EPOLLIN | EPOLLET;
                epoll_ctl(epfd, EPOLL_CTL_ADD, clntfd, &ev);
            }
            else if (events[i].events & EPOLLIN)
            {

                int readfd = events[i].data.fd;
                while (1)
                {
                    char buf[1024];
                    ssize_t read_bytes = read(readfd, buf, sizeof(buf));

                    if (read_bytes > 0)
                    {
                        printf("%s \n", buf);
                        write(readfd, buf, sizeof(buf));
                    }
                    else if (read_bytes == 0)
                    {
                        printf("client disconnect!");
                        close(readfd);
                        break;
                    }
                    else if (read_bytes == -1 && errno == EINTR) // 客户端正常中断
                    {
                        printf("continue reading");
                        continue;
                    }
                    else if (read_bytes == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK)))
                    {
                        printf("finish reading once,errno:%d \n", errno);
                        break;
                    }
                }
            }
            else
            {
                printf("do else thing");
            }
        }
    }

    close(sockfd);
    return 0;
}