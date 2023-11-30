#pragma once

#include "UnCopyable.h"

#include <functional>
#include <thread>
#include <memory>
#include <unistd.h>
#include <string>
#include <atomic>

class Thread : UnCopyable
{
public:
    using ThreadFunc = std::function<void()>;

    explicit Thread(ThreadFunc, const std::string &name = std::string());
    ~Thread();

    void start();
    void join();

    bool started() const { return m_started; }
    pid_t tid() const { return m_tid; }
    const std::string &name() const { return m_name; }

    static int numCreated() { return m_numCreated; }

private:
    void setDefaultName();

    bool m_started;
    bool m_joined;
    std::shared_ptr<std::thread> m_thread;
    pid_t m_tid;
    ThreadFunc m_func;
    std::string m_name;
    static std::atomic_int m_numCreated;
};