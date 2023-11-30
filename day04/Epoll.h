#pragma once
#include <sys/epoll.h>
#include <vector>

class Epoll
{
public:
    Epoll();
    ~Epoll();

    void addfd(int fd, int ctl, uint32_t event);
    std::vector<struct epoll_event> &poll();

private:
    int epfd;
    std::vector<struct epoll_event> activeEvents;
};