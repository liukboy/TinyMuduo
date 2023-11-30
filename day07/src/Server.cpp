#include "Server.h"
#include "Acceptor.h"
#include <functional>
#include <string.h>
#include <unistd.h>
#include <iostream>
#define READ_BUFFER 1024
Server::Server(EventLoop *loop_) : m_loop(loop_)
{
    // 连接socket，也就是accrptor，加到epoll树
    Acceptor *acc = new Acceptor(loop_);
    std::function<void(Socket *)> cb = std::bind(&Server::newConnection, this, std::placeholders::_1);
    acc->setCallback(cb);
}

void Server::newConnection(Socket *serv_sock)
{
    Socket *clnt_sock = new Socket(serv_sock->accept());
    printf("new client fd %d! \n", clnt_sock->getFd());

    // 客户端连接加到epoll
    Channel *clnt_ch = new Channel(m_loop, clnt_sock->getFd());
    std::function<void()> cb = std::bind(&Server::handleReadEvent, this, clnt_sock->getFd());
    clnt_ch->setCallback(cb);
    clnt_ch->enableReading();
    std::cout << "clnt_ch fd:" << clnt_sock->getFd() << std::endl;
}

void Server::handleReadEvent(int sockfd)
{
    char buf[READ_BUFFER];
    while (true)
    { // 由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if (bytes_read > 0)
        {
            printf("message from client fd %d: %s\n", sockfd, buf);
            write(sockfd, buf, sizeof(buf));
        }
        else if (bytes_read == -1 && errno == EINTR)
        { // 客户端正常中断、继续读取
            printf("continue reading");
            continue;
        }
        else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK)))
        { // 非阻塞IO，这个条件表示数据全部读取完毕
            printf("finish reading once, errno: %d\n", errno);
            break;
        }
        else if (bytes_read == 0)
        { // EOF，客户端断开连接
            printf("EOF, client fd %d disconnected\n", sockfd);
            close(sockfd); // 关闭socket会自动将文件描述符从epoll树上移除
            break;
        }
    }
}