#pragma once
#include <sys/epoll.h>
#include <functional>

#include "Socket.h"

class EventLoop;
class Channel
{
public:
    Channel(EventLoop *loop_, int fd_);

    void setCallBack(std::function<void()> cb);
    void enableReading();
    void handleEvent();

    int getFd();
    uint32_t getEvents();
    uint32_t getRevents();
    bool getInEpoll();
    void setInEpoll();
    void setRevents(uint32_t);

private:
    EventLoop *loop;
    int fd;
    int events;
    int revents;
    bool inEpoll;
    std::function<void()> callback;
};