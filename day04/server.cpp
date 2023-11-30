#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "Socket.h"
#include "InetAddress.h"
#include "Epoll.h"

#define MAX_EVENT 1024

void handleReadEvent(int);

int main()
{
    Socket *serv_sock = new Socket();
    InetAddress *addr = new InetAddress("127.0.0.1", 8888);
    serv_sock->bind(addr);
    serv_sock->listen();
    serv_sock->setnonblock();
    serv_sock->setReuse();

    Epoll *epoll = new Epoll();
    epoll->addfd(serv_sock->getfd(), EPOLL_CTL_ADD, EPOLLIN | EPOLLET);
    while (1)
    {
        std::vector<struct epoll_event> &activeFds = epoll->poll();

        for (int i = 0; i < activeFds.size(); ++i)
        {
            if (activeFds[i].data.fd == serv_sock->getfd())
            {
                int clntfd = serv_sock->accept();
                errif(clntfd == -1, "clnt_addr failed");
                Socket *clnt_sock = new Socket(clntfd);
                clnt_sock->setnonblock();
                epoll->addfd(clntfd, EPOLL_CTL_ADD, EPOLLIN | EPOLLET);
            }
            else if (activeFds[i].events & EPOLLIN)
            {
                handleReadEvent(activeFds[i].data.fd);
            }
            else
            {
                printf("do else thing");
            }
        }
    }

    delete serv_sock;
    delete epoll;
    return 0;
}

void handleReadEvent(int sockfd)
{
    while (1)
    {
        char buf[1024];
        ssize_t read_bytes = read(sockfd, buf, sizeof(buf));

        if (read_bytes > 0)
        {
            printf("%s \n", buf);
            write(sockfd, buf, sizeof(buf));
        }
        else if (read_bytes == 0)
        {
            printf("client disconnect!");
            close(sockfd);
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