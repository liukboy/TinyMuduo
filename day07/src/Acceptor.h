#pragma once
#include "EventLoop.h"
#include <functional>
#include "Socket.h"
/*
    建立连接的类
*/
class Acceptor
{
private:
    EventLoop *m_loop;
    Channel *m_accChannel;
    Socket *serv_sock;

public:
    Acceptor(EventLoop *loop_);
    ~Acceptor();

    void acceptConnection();
    std::function<void(Socket *)> newConnectionCallback;
    void setCallback(std::function<void(Socket *)> cb);
};
