#pragma once
#include <arpa/inet.h>

class InetAddress
{
public:
    InetAddress();
    InetAddress(const char *ip, uint16_t port);
    struct sockaddr_in &getAddr()
    {
        return addr;
    };

    socklen_t &getAddrLen()
    {
        return addr_len;
    }

private:
    struct sockaddr_in addr;
    socklen_t addr_len;
};