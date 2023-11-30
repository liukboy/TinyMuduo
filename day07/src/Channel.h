#pragma once
#include "EventLoop.h"
#include <functional>
/*
    封装socket + handleEvent
*/
class Channel
{
public:
    Channel(EventLoop *loop_, int fd_);

    void setCallback(std::function<void()> cb);
    void handleEvent();

    void enableReading();
    int getFd()
    {
        return m_fd;
    }

    int getEvents()
    {
        return m_events;
    }
    int getRevents()
    {
        return m_revents;
    }

    bool getInEpoll()
    {
        return m_inLoop;
    }

    void setInEpoll(bool inEpoll)
    {
        m_inLoop = inEpoll;
    }

    void setEvents(int events)
    {
        m_events = events;
    }

private:
    EventLoop *m_loop;
    int m_fd;
    std::function<void()> callback;
    bool m_inLoop;
    int m_events;
    int m_revents;
};