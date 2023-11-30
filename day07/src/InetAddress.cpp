#include "InetAddress.h"

InetAddress::InetAddress() : addr_len(sizeof(addr))
{
}
InetAddress::InetAddress(const char *ip, uint16_t port)
{
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);
    addr_len = sizeof(addr);
}