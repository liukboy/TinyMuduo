#pragma once
#include "Epoll.h"

class EventLoop
{
public:
    EventLoop();
    ~EventLoop();

    // 循环检测 epoll上有没有事件
    void loop();
    // 添加fd
    void addChannel(Channel *ch);

private:
    Epoll *ep;
    bool quit;
};