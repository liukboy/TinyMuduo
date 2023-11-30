#pragma once

/**
 * 用户使用muduo编写服务器程序
 */
#include "EventLoop.h"
#include "Acceptor.h"
#include "InetAddress.h"
#include "UnCopyable.h"
#include "EventLoopThreadPool.h"
#include "Callbacks.h"
#include "TcpConnection.h"
#include "Buffer.h"

#include <functional>
#include <string>
#include <memory>
#include <atomic>
#include <unordered_map>

// 对外的服务器编程使用的类
class TcpServer : UnCopyable
{
public:
    using ThreadInitCallback = std::function<void(EventLoop *)>;

    enum Option
    {
        kNoReusePort,
        kReusePort,
    };

    TcpServer(EventLoop *loop,
              const InetAddress &listenAddr,
              const std::string &nameArg,
              Option option = kNoReusePort);
    ~TcpServer();

    void setThreadInitcallback(const ThreadInitCallback &cb) { m_threadInitCallback = cb; }
    void setConnectionCallback(const ConnectionCallback &cb) { m_connectionCallback = cb; }
    void setMessageCallback(const MessageCallback &cb) { m_messageCallback = cb; }
    void setWriteCompleteCallback(const WriteCompleteCallback &cb) { m_writeCompleteCallback = cb; }

    // 设置底层subloop的个数
    void setThreadNum(int numThreads);

    // 开启服务器监听
    void start();

private:
    void newConnection(int sockfd, const InetAddress &peerAddr);
    void removeConnection(const TcpConnectionPtr &conn);
    void removeConnectionInLoop(const TcpConnectionPtr &conn);

    using ConnectionMap = std::unordered_map<std::string, TcpConnectionPtr>;

    EventLoop *m_loop; // baseLoop 用户定义的loop

    const std::string m_ipPort;
    const std::string m_name;

    std::unique_ptr<Acceptor> m_acceptor; // 运行在mainLoop，任务就是监听新连接事件

    std::shared_ptr<EventLoopThreadPool> m_threadPool; // one loop per thread

    ConnectionCallback m_connectionCallback;       // 有新连接时的回调
    MessageCallback m_messageCallback;             // 有读写消息时的回调
    WriteCompleteCallback m_writeCompleteCallback; // 消息发送完成以后的回调

    ThreadInitCallback m_threadInitCallback; // loop线程初始化的回调

    std::atomic_int m_started;

    int m_nextConnId;
    ConnectionMap m_connections; // 保存所有的连接
};