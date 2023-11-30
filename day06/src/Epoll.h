#pragma once
#include <sys/epoll.h>
#include <vector>
#include "Channel.h"

class Epoll
{
public:
    Epoll();
    void updateChannel(Channel *);
    std::vector<Channel *> poll(int timeout = -1);
    void addFd(int fd, uint32_t op);

private:
    int epfd;
    struct epoll_event *events;
};