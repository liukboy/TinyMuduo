#pragma once
#include <iostream>
#include <vector>
#include "sys/epoll.h"

class Channel;
class Epoll
{
public:
    Epoll();
    // epoll_wait函数封装，返回事件channel
    std::vector<Channel *> poll(int timeout);

    // epoll添加socket(也就是channel)
    void addChannel(Channel* ch);

private:
    int epfd;
    struct epoll_event *events;
};
