/*
 * @Author: liukboy wowkliu@163.com
 * @Date: 2023-10-31 19:06:59
 * @Description: 异步日志类
 *
 * Copyright (c) 2023 by liukboy, All Rights Reserved.
 */
#include "LogBuffer.h"
#include <memory>
#include <vector>
#include <string>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <functional>

using namespace std;
const int AsyncBufferSize = 4 * 1024 * 1024; // 前端每个buffer的大小 4M.当写入4M的数据的时候才写文件
class AsyncLogging
{
public:
    using Buffer = LogBuffer<AsyncBufferSize>;
    using BufferPtr = unique_ptr<Buffer>;
    using BufferVec = vector<BufferPtr>;

public:
    AsyncLogging(const string &filename, size_t rollsize);
    ~AsyncLogging()
    {
        if (m_running)
        {
            stop();
        }
    }
    void stop()
    {
        if (m_running)
        {
            m_running = false;
            m_cv.notify_one();
            m_thread->join();
        }
    }
    void start();
    // 添加一条日志到AsyncBuffer
    void append(const char *line, size_t len);

private:
    void thread_fun();
    BufferPtr m_curbuffer;  // 当前存放日志buffer
    BufferPtr m_nextbuffer; // 备选buffer
    BufferVec m_buffer_vec; // 准备要写入文件的buffer集合

    string m_filename; // 日志文件名
    size_t m_rollsize; // 日志文件容量阈值，超过就新建一个新文件

    unique_ptr<thread> m_thread;
    mutex m_mutex;
    condition_variable m_cv;

    bool m_running; // 线程终止标志

    const int g_cv_wait_interval = 3;
};