#pragma once

#include <vector>
#include <string>
#include <algorithm>

// 网络库底层的缓冲器类型定义
class Buffer
{
public:
    static const size_t kCheapPrepend = 8;
    static const size_t kInitialSize = 1024;

    explicit Buffer(size_t initialSize = kInitialSize)
        : m_buffer(kCheapPrepend + initialSize),
          m_readerIndex(kCheapPrepend),
          m_writerIndex(kCheapPrepend)
    {
    }

    size_t readableBytes() const
    {
        return m_writerIndex - m_readerIndex;
    }

    size_t writableBytes() const
    {
        return m_buffer.size() - m_writerIndex;
    }

    size_t prependableBytes() const
    {
        return m_readerIndex;
    }

    // 返回缓冲区中可读数据的起始地址
    const char *peek() const
    {
        return begin() + m_readerIndex;
    }

    // onMessage string <- Buffer
    void retrieve(size_t len)
    {
        if (len < readableBytes())
        {
            m_readerIndex += len; // 应用只读取了刻度缓冲区数据的一部分，就是len，还剩下readerIndex_ += len -> m_writerIndex
        }
        else // len == readableBytes()
        {
            retrieveAll();
        }
    }

    void retrieveAll()
    {
        m_readerIndex = m_writerIndex = kCheapPrepend;
    }

    // 把onMessage函数上报的Buffer数据，转成string类型的数据返回
    std::string retrieveAllAsString()
    {
        return retrieveAsString(readableBytes()); // 应用可读取数据的长度
    }

    std::string retrieveAsString(size_t len)
    {
        std::string result(peek(), len);
        retrieve(len); // 上面一句把缓冲区中可读的数据，已经读取出来，这里肯定要对缓冲区进行复位操作
        return result;
    }

    // m_buffer.size() - m_writerIndex    len
    void ensureWriteableBytes(size_t len)
    {
        if (writableBytes() < len)
        {
            makeSpace(len); // 扩容函数
        }
    }

    // 把[data, data+len]内存上的数据，添加到writable缓冲区当中
    void append(const char *data, size_t len)
    {
        ensureWriteableBytes(len);
        std::copy(data, data + len, beginWrite());
        m_writerIndex += len;
    }

    char *beginWrite()
    {
        return begin() + m_writerIndex;
    }

    const char *beginWrite() const
    {
        return begin() + m_writerIndex;
    }

    // 从fd上读取数据
    ssize_t readFd(int fd, int *saveErrno);
    // 通过fd发送数据
    ssize_t writeFd(int fd, int *saveErrno);

private:
    char *begin()
    {
        // it.operator*()
        return &*m_buffer.begin(); // vector底层数组首元素的地址，也就是数组的起始地址
    }
    const char *begin() const
    {
        return &*m_buffer.begin();
    }
    void makeSpace(size_t len)
    {
        if (writableBytes() + prependableBytes() < len + kCheapPrepend)
        {
            m_buffer.resize(m_writerIndex + len);
        }
        else
        {
            size_t readalbe = readableBytes();
            std::copy(begin() + m_readerIndex,
                      begin() + m_writerIndex,
                      begin() + kCheapPrepend);
            m_readerIndex = kCheapPrepend;
            m_writerIndex = m_readerIndex + readalbe;
        }
    }

    std::vector<char> m_buffer;
    size_t m_readerIndex;
    size_t m_writerIndex;
};