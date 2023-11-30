#include "Acceptor.h"
#include "Channel.h"

Acceptor::Acceptor(EventLoop *loop_) : m_loop(loop_)
{
    serv_sock = new Socket();
    InetAddress *addr = new InetAddress("127.0.0.1", 8888);
    serv_sock->bind(addr);
    serv_sock->listen();

    m_accChannel = new Channel(loop_, serv_sock->getFd());
    std::function<void()> cb = std::bind(&Acceptor::acceptConnection, this);
    m_accChannel->setCallback(cb);
    m_accChannel->enableReading();
}

Acceptor::~Acceptor()
{
}

void Acceptor::acceptConnection()
{
    newConnectionCallback(serv_sock);
}

void Acceptor::setCallback(std::function<void(Socket *)> cb)
{
    newConnectionCallback = cb;
}