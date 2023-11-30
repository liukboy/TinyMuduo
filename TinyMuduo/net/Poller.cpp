#include "Poller.h"
#include "Channel.h"

Poller::Poller(EventLoop *loop)
    : m_ownerLoop(loop)
{
}

bool Poller::hasChannel(Channel *channel) const
{
    auto it = m_channelMap.find(channel->fd());
    return it != m_channelMap.end() && it->second == channel;
}