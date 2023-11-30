/*
 * @Author: liukboy wowkliu@163.com
 * @Date: 2023-11-06 14:28:24
 * @Description: 封装socketfd 和 感兴趣的事件，最重要的是绑定了事件处理函数。epoll
 *
 * Copyright (c) 2023 by liukboy, All Rights Reserved.
 */

#pragma once

#include "UnCopyable.h"
#include "Timestamp.h"

#include <functional>
#include <memory>

class EventLoop;

class Channel : UnCopyable
{
public:
    using EventCallback = std::function<void()>; // 定义回调函数
    using ReadEventCallback = std::function<void(Timestamp)>;

    Channel(EventLoop *loop, int fd);
    ~Channel();

    // fd得到poller通知以后，处理事件的
    void handleEvent(Timestamp receiveTime);

    // 设置回调函数对象
    void setReadCallback(ReadEventCallback cb) { m_readCallback = std::move(cb); }
    void setWriteCallback(EventCallback cb) { m_writeCallback = std::move(cb); }
    void setCloseCallback(EventCallback cb) { m_closeCallback = std::move(cb); }
    void setErrorCallback(EventCallback cb) { m_errorCallback = std::move(cb); }

    // 防止当channel被手动remove掉，channel还在执行回调操作
    void tie(const std::shared_ptr<void> &);

    int fd() const { return m_fd; }
    int events() const { return m_events; }
    int set_revents(int revt) { m_revents = revt; }

    // 设置fd相应的事件状态
    void enableReading()
    {
        m_events |= kReadEvent;
        update();
    }
    void disableReading()
    {
        m_events &= ~kReadEvent;
        update();
    }
    void enableWriting()
    {
        m_events |= kWriteEvent;
        update();
    }
    void disableWriting()
    {
        m_events &= ~kWriteEvent;
        update();
    }
    void disableAll()
    {
        m_events = kNoneEvent;
        update();
    }

    // 返回fd当前的事件状态
    bool isNoneEvent() const { return m_events == kNoneEvent; }
    bool isWriting() const { return m_events & kWriteEvent; }
    bool isReading() const { return m_events & kReadEvent; }

    int index() { return m_index; }
    void set_index(int idx) { m_index = idx; }

    // one loop per thread
    EventLoop *ownerLoop() { return m_loop; }
    void remove();

private:
    void update();
    void handleEventWithGuard(Timestamp receiveTime);

    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;

    EventLoop *m_loop; // 事件循环,就是对应一个epoll对象
    const int m_fd;    // epoll监听的socket fd
    int m_events;      // 注册fd感兴趣的事件
    int m_revents;     // epoll_wait返回的具体发生的事件
    int m_index;

    std::weak_ptr<void> tie_;
    bool tied_;

    // m_revents返回的事件处理函数
    ReadEventCallback m_readCallback;
    EventCallback m_writeCallback;
    EventCallback m_closeCallback;
    EventCallback m_errorCallback;
};
