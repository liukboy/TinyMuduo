/*
 * @Author: liukboy wowkliu@163.com
 * @Date: 2023-10-31 07:33:30
 * @Description: 日志buffer，其实就是一个char[],日志内容都存在这里
 *
 * Copyright (c) 2023 by liukboy, All Rights Reserved.
 */
#pragma once
#include <string.h>

template <unsigned int SIZE>
class LogBuffer
{
public:
    LogBuffer() : m_cur(m_buffer) {}
    void append(const char *buf, size_t len)
    {
        if (end() - m_cur > len)
        {
            strcpy(m_cur, buf);
            m_cur += len;
        }
    }
    /**
     * @description: 返回数组当前位置，用于加入数据
     * @return {*}
     */
    char *cur()
    {
        return m_cur;
    }

    char *end()
    {
        return m_buffer + SIZE * sizeof(char);
    }

    void add(size_t len)
    {
        m_cur += len;
    }

    const char *data()
    {
        return m_buffer;
    }

    size_t len()
    {
        return m_cur - m_buffer;
    }

    void reset()
    {
        m_cur = m_buffer;
    }
    void bzero()
    {
        ::bzero(m_buffer, sizeof(m_buffer));
    }

    size_t avil()
    {
        return end() - m_cur;
    }

private:
    char m_buffer[SIZE];
    char *m_cur; // 当前位置
};