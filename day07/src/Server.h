#pragma once
#include "EventLoop.h"
#include "Channel.h"
#include "Socket.h"

class Server
{
public:
    Server(EventLoop *loop_);

    void newConnection(Socket *serv_sock);
    void handleReadEvent(int sockfd);

private:
    EventLoop *m_loop;
};