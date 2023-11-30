#pragma once

#include <sys/socket.h>
#include "InetAddress.h"

class Socket
{
private:
    int m_fd;

public:
    Socket(/* args */);
    Socket(int fd_);
    ~Socket();
    void bind(InetAddress *addr);
    void listen();
    int accept();

    int getFd()
    {
        return m_fd;
    }
};
