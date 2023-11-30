/*
 * @Author: liukboy wowkliu@163.com
 * @Date: 2023-10-31 07:43:24
 * @Description: 包含一个LogBuffer，重载 <<,流式输入数字，字符串到LogBuffer
 *
 * Copyright (c) 2023 by liukboy, All Rights Reserved.
 */

#include "LogBuffer.h"

#include <string>

using namespace std;
const int kStreamBufferSize = 4 * 1000;
const int kMaxNumericSize = 48;
class LogStream
{
public:
    using Buffer = LogBuffer<kStreamBufferSize>;

public:
    LogStream() = default;
    ~LogStream() = default;

    void append(const char *buf);

    // 能想到的一些类型
    LogStream &operator<<(bool val);
    LogStream &operator<<(char ch);
    LogStream &operator<<(int val);
    LogStream &operator<<(size_t val);
    LogStream &operator<<(ssize_t val);
    LogStream &operator<<(double val);
    LogStream &operator<<(const char *str);
    LogStream &operator<<(const string &str);

    Buffer &get_buffer()
    {
        return m_buffer;
    }

private:
    Buffer m_buffer;
};