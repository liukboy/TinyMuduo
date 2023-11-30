#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "util.h"

class InetAddress;
class Socket
{
public:
    Socket();
    ~Socket();

    Socket(int fd_);
    void close();

    void bind(InetAddress *addr);
    void listen();
    int accept();
    void setnonblock();
    void setReuse();

    int getfd()
    {
        return sockfd;
    }

private:
    int sockfd;
};

#endif