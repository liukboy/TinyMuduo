#include "Channel.h"

Channel::Channel(EventLoop *loop_, int fd_) : m_loop(loop_), m_fd(fd_), m_inLoop(false)
{
}

void Channel::setCallback(std::function<void()> cb)
{
    callback = cb;
}

void Channel::handleEvent()
{
    callback();
}

void Channel::enableReading()
{
    m_events = EPOLLIN | EPOLLET;
    m_loop->addChannel(this);
}