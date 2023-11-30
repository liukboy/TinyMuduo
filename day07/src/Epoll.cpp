#include "Epoll.h"
#include "util.h"
#include "Channel.h"
#include <iostream>

#define MAX_EVENT 1024
Epoll::Epoll()
{
    epfd = epoll_create(5);
    events = new struct epoll_event[MAX_EVENT];
}

std::vector<Channel *> Epoll::poll(int timeout)
{
    std::vector<Channel *> activeChs;
    int fds = epoll_wait(epfd, events, MAX_EVENT, timeout);
    errif(fds == -1, "epoll_wait failed");
    for (int i = 0; i < fds; i++)
    {
        activeChs.push_back((Channel *)events[i].data.ptr);
        std::cout << "fds:" << i << std::endl;
    }
    return activeChs;
}

void Epoll::addChannel(Channel *ch)
{
    int chfd = ch->getFd();
    struct epoll_event event;
    event.data.ptr = ch;
    event.events = ch->getEvents();
    bool inEpoll = ch->getInEpoll();
    if (!inEpoll)
    {
        ch->setInEpoll(true);
        errif(epoll_ctl(epfd, EPOLL_CTL_ADD, chfd, &event) == -1, "epoll_ctl failed");
    }
    else
    {
        errif(epoll_ctl(epfd, EPOLL_CTL_MOD, chfd, &event) == -1, "epoll_ctl failed");
    }
}