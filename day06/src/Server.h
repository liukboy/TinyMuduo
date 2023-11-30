#pragma once

class EventLoop;
class Socket;
class Server
{
public:
    Server(EventLoop *loop);

    void newConnection(Socket *serv_sock);
    void handleReadEvent(int);

private:
    EventLoop *loop;
};