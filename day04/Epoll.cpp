#include "Epoll.h"
#include "util.h"
#include <unistd.h>

#define MAX_EVENT 1024
Epoll::Epoll()
{
    epfd = epoll_create(5);
    errif(epfd == -1, "epoll create failed");
}

Epoll::~Epoll()
{
    if (epfd != -1)
    {
        ::close(epfd);
        epfd = -1;
    }
}

void Epoll::addfd(int fd, int ctl, uint32_t event)
{
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = event;
    errif(::epoll_ctl(epfd, ctl, fd, &ev) == -1, "addfd failed");
}

std::vector<struct epoll_event> &Epoll::poll()
{
    struct epoll_event ev, events[MAX_EVENT];
    int fds = epoll_wait(epfd, events, MAX_EVENT, -1);
    activeEvents.clear();
    for (int i = 0; i < fds; i++)
    {
        activeEvents.push_back(events[i]);
    }
    return activeEvents;
}