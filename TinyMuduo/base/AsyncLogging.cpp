#include "AsyncLogging.h"
#include "LogFile.h"

#include <utility>
#include <cassert>
#include <chrono>

AsyncLogging::AsyncLogging(const string &filename, size_t rollsize)
    : m_filename(filename),
      m_rollsize(rollsize),
      m_curbuffer(make_unique<Buffer>()),
      m_nextbuffer(make_unique<Buffer>()),
      m_buffer_vec()
{
    m_curbuffer->bzero();
    m_nextbuffer->bzero();
    m_buffer_vec.reserve(16);
}

void AsyncLogging::start()
{
    m_running = true;
    m_thread = make_unique<thread>(&AsyncLogging::thread_fun, this);
}

void AsyncLogging::append(const char *line, size_t len)
{
    lock_guard<mutex> lg(m_mutex);
    if (m_curbuffer->avil() > len)
    {
        m_curbuffer->append(line, len);
    }
    else
    {
        // curbuffer 满了，那么需要把它放到vec中去，并且cur指向新的buffer
        m_buffer_vec.emplace_back(m_curbuffer.release());
        if (m_nextbuffer)
        {
            m_curbuffer = move(m_nextbuffer);
        }
        else
        {
            m_curbuffer = make_unique<Buffer>();
        }
        m_curbuffer->append(line, len);
        m_cv.notify_one();
    }
}

void AsyncLogging::thread_fun()
{
    LogFile outfile(m_filename, m_rollsize);
    BufferPtr buffer_a = make_unique<Buffer>();
    BufferPtr buffer_b = make_unique<Buffer>();
    BufferVec write_buffer_vec;
    write_buffer_vec.reserve(16);

    while (m_running)
    {
        {
            unique_lock<mutex> ulock(m_mutex);
            if (m_buffer_vec.empty()) // 这里为什么不用while还需要想一下
            {
                m_cv.wait_for(ulock, std::chrono::seconds(g_cv_wait_interval));
            }
            m_buffer_vec.emplace_back(m_curbuffer.release());
            m_curbuffer = move(buffer_a);
            write_buffer_vec.swap(m_buffer_vec);
            if (!m_nextbuffer)
            {
                m_nextbuffer = move(buffer_b);
            }
        }
        // 遍历日志数组，写到文件中去
        for (const auto &buf : write_buffer_vec)
        {
            outfile.append(buf->data(), buf->len());
        }
        //
        if (write_buffer_vec.size() > 2)
        {
            write_buffer_vec.resize(2);
        }
        if (!buffer_a)
        {
            buffer_a = std::move(write_buffer_vec.back());
            write_buffer_vec.pop_back();
            buffer_a->reset(); // reset buffer_a
        }
        if (!buffer_b)
        {
            // 说明 buffer_b 给了 m_nextbuffer, write_buffer_vec 必然还有 1 个 buffer
            buffer_b = std::move(write_buffer_vec.back());
            write_buffer_vec.pop_back();
            buffer_b->reset();
        }
        write_buffer_vec.clear();
        outfile.flush();
    }
    outfile.flush();
}
