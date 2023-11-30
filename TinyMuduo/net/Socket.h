#pragma once

#include "UnCopyable.h"

class InetAddress;

// 封装socket fd
class Socket : UnCopyable
{
public:
    explicit Socket(int sockfd)
        : m_sockfd(sockfd)
    {
    }

    ~Socket();

    int fd() const { return m_sockfd; }
    void bindAddress(const InetAddress &localaddr);
    void listen();
    int accept(InetAddress *peeraddr);

    void shutdownWrite();

    void setTcpNoDelay(bool on);
    void setReuseAddr(bool on);
    void setReusePort(bool on);
    void setKeepAlive(bool on);

private:
    const int m_sockfd;
};