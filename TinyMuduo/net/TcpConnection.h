#pragma once

#include "UnCopyable.h"
#include "InetAddress.h"
#include "Callbacks.h"
#include "Buffer.h"
#include "Timestamp.h"

#include <memory>
#include <string>
#include <atomic>

class Channel;
class EventLoop;
class Socket;

/**
 * TcpServer => Acceptor => 有一个新用户连接，通过accept函数拿到connfd
 * =》 TcpConnection 设置回调 =》 Channel =》 Poller =》 Channel的回调操作
 *
 */
class TcpConnection : UnCopyable, public std::enable_shared_from_this<TcpConnection>
{
public:
    TcpConnection(EventLoop *loop,
                  const std::string &name,
                  int sockfd,
                  const InetAddress &localAddr,
                  const InetAddress &peerAddr);
    ~TcpConnection();

    EventLoop *getLoop() const { return m_loop; }
    const std::string &name() const { return m_name; }
    const InetAddress &localAddress() const { return m_localAddr; }
    const InetAddress &peerAddress() const { return m_peerAddr; }

    bool connected() const { return m_state == kConnected; }

    // 发送数据
    void send(const std::string &buf);
    // 关闭连接
    void shutdown();

    void setConnectionCallback(const ConnectionCallback &cb)
    {
        m_connectionCallback = cb;
    }

    void setMessageCallback(const MessageCallback &cb)
    {
        m_messageCallback = cb;
    }

    void setWriteCompleteCallback(const WriteCompleteCallback &cb)
    {
        m_writeCompleteCallback = cb;
    }

    void setHighWaterMarkCallback(const HighWaterMarkCallback &cb, size_t highWaterMark)
    {
        m_highWaterMarkCallback = cb;
        m_highWaterMark = highWaterMark;
    }

    void setCloseCallback(const CloseCallback &cb)
    {
        m_closeCallback = cb;
    }

    // 连接建立
    void connectEstablished();
    // 连接销毁
    void connectDestroyed();

private:
    enum StateE
    {
        kDisconnected,
        kConnecting,
        kConnected,
        kDisconnecting
    };
    void setState(StateE state) { m_state = state; }

    void handleRead(Timestamp receiveTime);
    void handleWrite();
    void handleClose();
    void handleError();

    void sendInLoop(const void *message, size_t len);
    void shutdownInLoop();

    EventLoop *m_loop; // 这里绝对不是baseLoop， 因为TcpConnection都是在subLoop里面管理的
    const std::string m_name;
    std::atomic_int m_state;
    bool m_reading;

    // 这里和Acceptor类似   Acceptor=》mainLoop    TcpConenction=》subLoop
    std::unique_ptr<Socket> m_socket;
    std::unique_ptr<Channel> m_channel;

    const InetAddress m_localAddr;
    const InetAddress m_peerAddr;

    ConnectionCallback m_connectionCallback;       // 有新连接时的回调
    MessageCallback m_messageCallback;             // 有读写消息时的回调
    WriteCompleteCallback m_writeCompleteCallback; // 消息发送完成以后的回调
    HighWaterMarkCallback m_highWaterMarkCallback;
    CloseCallback m_closeCallback;
    size_t m_highWaterMark;

    Buffer m_inputBuffer;  // 接收数据的缓冲区
    Buffer m_outputBuffer; // 发送数据的缓冲区
};