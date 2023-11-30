#pragma once

class InetAddress;
class Socket
{
public:
    Socket();
    Socket(int);

    void bind(InetAddress *addr);
    void listen();
    void setnonblocking();

    int accept(InetAddress *);

    int getfd()
    {
        return fd;
    }

private:
    int fd;
};