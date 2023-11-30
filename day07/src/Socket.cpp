#include "Socket.h"
#include "util.h"

Socket::Socket(/* args */)
{
    m_fd = socket(AF_INET, SOCK_STREAM, 0);
    errif(m_fd == -1, "socket failed");
}

Socket::Socket(int fd_) : m_fd(fd_)
{
}

Socket::~Socket()
{
}

void Socket::bind(InetAddress *addr)
{
    errif(::bind(m_fd, (sockaddr *)&addr->getAddr(), addr->getAddrLen()) == -1, "bind failed");
}

void Socket::listen()
{
    errif(::listen(m_fd, 5) == -1, "listen failed");
}

int Socket::accept()
{
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);

    int clntfd = ::accept(m_fd, (sockaddr *)&addr, &addrlen);

    return clntfd;
}
