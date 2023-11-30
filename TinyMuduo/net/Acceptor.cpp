#include "Acceptor.h"
#include "Logger.h"
#include "InetAddress.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>

static int createNonblocking()
{
    int sockfd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
    if (sockfd < 0)
    {
        LOG_ERROR << "listen socket create err:" << errno;
    }
    return sockfd;
}

Acceptor::Acceptor(EventLoop *loop, const InetAddress &listenAddr, bool reuseport)
    : m_loop(loop),
      m_acceptSocket(createNonblocking()), // socket
      m_acceptChannel(loop, m_acceptSocket.fd()),
      m_listenning(false)
{
    m_acceptSocket.setReuseAddr(true);
    m_acceptSocket.setReusePort(true);
    m_acceptSocket.bindAddress(listenAddr); // bind
    // TcpServer::start() Acceptor.listen  有新用户的连接，要执行一个回调（connfd=》channel=》subloop）
    // baseLoop => m_acceptChannel(listenfd) =>
    m_acceptChannel.setReadCallback(std::bind(&Acceptor::handleRead, this));
}

Acceptor::~Acceptor()
{
    m_acceptChannel.disableAll();
    m_acceptChannel.remove();
}

void Acceptor::listen()
{
    m_listenning = true;
    m_acceptSocket.listen();         // listen
    m_acceptChannel.enableReading(); // m_acceptChannel => Poller
}

// listenfd有事件发生了，就是有新用户连接了
void Acceptor::handleRead()
{
    InetAddress peerAddr;
    int connfd = m_acceptSocket.accept(&peerAddr);
    if (connfd >= 0)
    {
        if (m_newConnectionCallback)
        {
            m_newConnectionCallback(connfd, peerAddr); // 轮询找到subLoop，唤醒，分发当前的新客户端的Channel
        }
        else
        {
            ::close(connfd);
        }
    }
    else
    {
        LOG_ERROR << "accept err:" << errno;
        if (errno == EMFILE)
        {
            LOG_ERROR << "sockfd reached limit! ";
        }
    }
}