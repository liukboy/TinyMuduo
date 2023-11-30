#include "EventLoop.h"
#include "Epoll.h"
#include "Channel.h"

EventLoop::EventLoop() : ep(nullptr), quit(false)
{
    ep = new Epoll();
}

EventLoop::~EventLoop()
{
    if (ep)
    {
        delete ep;
    }
}

void EventLoop::loop()
{
    while (!quit)
    {
        // 返回事件，就是一系列的channel，然后channel调用处理函数
        std::vector<Channel *> activeChs = ep->poll(-1);
        for (auto it = activeChs.begin(); it != activeChs.end(); it++)
        {
            (*it)->handleEvent();
        }
    }
}

void EventLoop::addChannel(Channel *ch)
{
    ep->addChannel(ch);
}