#include "Socket.h"
#include "util.h"
#include "InetAddress.h"
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
Socket::Socket()
{
    fd = socket(AF_INET, SOCK_STREAM, 0);
    errif(fd == -1, "socket create failed");
}
Socket::Socket(int _fd) : fd(_fd)
{
    errif(fd == -1, "socket create error");
}

void Socket::bind(InetAddress *addr)
{
    errif(::bind(fd, (sockaddr *)&(addr->getAddr()), addr->getAddrLen()) == -1, "bind failed");
}

void Socket::listen()
{
    errif(::listen(fd, 5) == -1, "listen failed");
}

int Socket::accept(InetAddress *addr)
{
    int clnt_sockfd = ::accept(fd, (sockaddr *)&(addr->getAddr()), &addr->getAddrLen());
    errif(clnt_sockfd == -1, "socket accept error");
    return clnt_sockfd;
}

void Socket::setnonblocking()
{
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}