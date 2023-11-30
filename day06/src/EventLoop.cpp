#include "EventLoop.h"

EventLoop::EventLoop() : ep(nullptr)
{
    ep = new Epoll();
}

void EventLoop::updateChannel(Channel *ch)
{
    ep->updateChannel(ch);
}

void EventLoop::loop()
{
    while (!quit)
    {
        std::vector<Channel *> chs;
        chs = ep->poll();
        for (auto it = chs.begin(); it != chs.end(); ++it)
        {
            (*it)->handleEvent();
        }
    }
}