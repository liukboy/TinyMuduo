#pragma once

#include <functional>
#include <vector>
#include <atomic>
#include <memory>
#include <mutex>

#include "UnCopyable.h"
#include "Timestamp.h"
#include "CurrentThread.h"

class Channel;
class Poller;

// 时间循环类  主要包含了两个大模块 Channel   Poller（epoll的抽象）
class EventLoop : UnCopyable
{
public:
    using Functor = std::function<void()>;

    EventLoop();
    ~EventLoop();

    // 开启事件循环
    void loop();
    // 退出事件循环
    void quit();

    Timestamp pollReturnTime() const { return m_pollReturnTime; }

    // 在当前loop中执行cb
    void runInLoop(Functor cb);
    // 把cb放入队列中，唤醒loop所在的线程，执行cb
    void queueInLoop(Functor cb);

    // 用来唤醒loop所在的线程的
    void wakeup();

    // EventLoop的方法 =》 Poller的方法
    void updateChannel(Channel *channel);
    void removeChannel(Channel *channel);
    bool hasChannel(Channel *channel);

    // 判断EventLoop对象是否在自己的线程里面
    bool isInLoopThread() const { return m_threadId == CurrentThread::tid(); }

private:
    void handleRead();        // wake up
    void doPendingFunctors(); // 执行回调

    using ChannelList = std::vector<Channel *>;

    std::atomic_bool m_looping; // 原子操作，通过CAS实现的
    std::atomic_bool m_quit;    // 标识退出loop循环

    const pid_t m_threadId; // 记录当前loop所在线程的id

    Timestamp m_pollReturnTime; // poller返回发生事件的channels的时间点
    std::unique_ptr<Poller> m_poller;

    int m_wakeupFd; // 主要作用，当mainLoop获取一个新用户的channel，通过轮询算法选择一个subloop，通过该成员唤醒subloop处理channel
    std::unique_ptr<Channel> m_wakeupChannel;

    ChannelList m_activeChannels;

    std::atomic_bool m_callingPendingFunctors; // 标识当前loop是否有需要执行的回调操作
    std::vector<Functor> m_pendingFunctors;    // 存储loop需要执行的所有的回调操作
    std::mutex m_mutex;                        // 互斥锁，用来保护上面vector容器的线程安全操作
};