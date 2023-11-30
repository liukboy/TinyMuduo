#include "Channel.h"
#include "EventLoop.h"

Channel::Channel(EventLoop *loop_, int fd_) : loop(loop_), fd(fd_)
{
}

void Channel::setCallBack(std::function<void()> cb)
{
    callback = cb;
}

void Channel::enableReading()
{
    events = EPOLLIN | EPOLLET;
    loop->updateChannel(this);
}

int Channel::getFd()
{
    return fd;
}

uint32_t Channel::getEvents()
{
    return events;
}

uint32_t Channel::getRevents()
{
    return revents;
}

bool Channel::getInEpoll()
{
    return inEpoll;
}

void Channel::setInEpoll()
{
    inEpoll = true;
}

// void Channel::setEvents(uint32_t _ev){
//     events = _ev;
// }

void Channel::setRevents(uint32_t _ev)
{
    revents = _ev;
}

void Channel::handleEvent()
{
    callback();
}