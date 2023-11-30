#pragma once

#include "UnCopyable.h"
#include "Thread.h"

#include <functional>
#include <mutex>
#include <condition_variable>
#include <string>

class EventLoop;

class EventLoopThread : UnCopyable
{
public:
    using ThreadInitCallback = std::function<void(EventLoop *)>;

    EventLoopThread(const ThreadInitCallback &cb = ThreadInitCallback(),
                    const std::string &name = std::string());
    ~EventLoopThread();

    EventLoop *startLoop();

private:
    void threadFunc();

    EventLoop *m_loop;
    bool m_exiting;
    Thread m_thread;
    std::mutex m_mutex;
    std::condition_variable m_cond;
    ThreadInitCallback m_callback;
};