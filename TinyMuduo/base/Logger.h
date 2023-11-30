/*
 * @Author: liukboy wowkliu@163.com
 * @Date: 2023-10-31 08:08:34
 * @Description: 日志对象，每一条日志都会新建一个Logger对象，成员变量包含一个stream,
 *  内容都会添加到stream中，然后析构函数的时候会把stream中的数据输出
 *
 * Copyright (c) 2023 by liukboy, All Rights Reserved.
 */

#include "LogStream.h"

class Logger
{
public:
    enum LogLevel
    {
        DEBUG,
        INFO,
        WARN,
        ERROR,
        NUM_LEVEL
    };

    using OutputFun = void (*)(const char *, size_t);
    using FlushFun = void (*)();

public:
    Logger(const char *filename, int num, int level);
    ~Logger();

    LogStream &stream()
    {
        return m_stream;
    }

    void append_prefix();
    void append_time();
    void append_level();
    void append_end();

    static void set_output_fun(OutputFun fun) { g_output_fun = fun; }
    static void set_flush_fun(FlushFun fun) { g_flush_fun = fun; }
    static void default_output_fun(const char *, size_t);
    static void default_flush_fun();

private:
    static OutputFun g_output_fun;
    static FlushFun g_flush_fun;
    LogStream m_stream;
    const char *m_filename; // 日志所在文件的名字
    int m_line;             // 日志所在行数
    int m_level;

    static const int MicrosecondsPerSecond = 1000 * 1000;
};

#define LOG_DEBUG \
    Logger(__FILE__, __LINE__, Logger::DEBUG).stream()

#define LOG_INFO \
    Logger(__FILE__, __LINE__, Logger::INFO).stream()

#define LOG_WARN \
    Logger(__FILE__, __LINE__, Logger::WARN).stream()

#define LOG_ERROR \
    Logger(__FILE__, __LINE__, Logger::ERROR).stream()
