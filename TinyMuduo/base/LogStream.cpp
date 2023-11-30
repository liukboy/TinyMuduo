#include "LogStream.h"

void LogStream::append(const char *buf)
{
    m_buffer.append(buf, strlen(buf));
}

LogStream &LogStream::operator<<(bool val)
{
    m_buffer.append(val ? "1" : "0", 1);
}

LogStream &LogStream::operator<<(char ch)
{
    m_buffer.append(&ch, 1);
}

LogStream &LogStream::operator<<(int val)
{
    int len = snprintf(m_buffer.cur(), kMaxNumericSize, "%d", val);
    m_buffer.add(len);
    return *this;
}

LogStream &LogStream::operator<<(size_t val)
{
    int len = snprintf(m_buffer.cur(), kMaxNumericSize, "%d", val);
    m_buffer.add(len);
    return *this;
}

LogStream &LogStream::operator<<(ssize_t val)
{
    int len = snprintf(m_buffer.cur(), kMaxNumericSize, "%d", val);
    m_buffer.add(len);
    return *this;
}

LogStream &LogStream::operator<<(double val)
{
    int len = snprintf(m_buffer.cur(), kMaxNumericSize, "%.12g", val);
    m_buffer.add(len);
    return *this;
}

LogStream &LogStream::operator<<(const char *str)
{
    m_buffer.append(str, strlen(str));
    return *this;
}

LogStream &LogStream::operator<<(const string &str)
{
    m_buffer.append(str.c_str(), str.size());
    return *this;
}
