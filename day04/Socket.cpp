#include "Socket.h"
#include "InetAddress.h"

Socket::Socket() : sockfd(-1)
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(sockfd == -1, "socket create failed!");
}

Socket::Socket(int fd_)
{
    sockfd = fd_;
}

Socket::~Socket()
{
    if (sockfd != -1)
    {
        close();
        sockfd = -1;
    }
}

void Socket::close()
{
    ::close(sockfd);
}

void Socket::bind(InetAddress *addr)
{
    errif(::bind(sockfd, (sockaddr *)&(addr->getAddr()), addr->getAddrLen()) == -1, "bind failed");
}

void Socket::listen()
{
    errif(::listen(sockfd, 5) == -1, "listen failed");
}

int Socket::accept()
{
    struct sockaddr_in clnt_addr;
    socklen_t clnt_len = sizeof(clnt_addr);
    int clnt_fd = ::accept(sockfd, (sockaddr *)&clnt_addr, &clnt_len);
    errif(clnt_fd == -1, "accept failed");
    return clnt_fd;
}

void Socket::setnonblock()
{
    ::fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL) | O_NONBLOCK);
}

void Socket::setReuse()
{
    int option = 1;
    ::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(int));
}