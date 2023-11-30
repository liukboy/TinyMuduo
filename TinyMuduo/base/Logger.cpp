#include "Logger.h"

#include <unistd.h>
#include <sys/syscall.h>
#include <sys/time.h>

#include <chrono>
#include <ctime>

Logger::OutputFun Logger::g_output_fun = Logger::default_output_fun;
Logger::FlushFun Logger::g_flush_fun = Logger::default_flush_fun;

Logger::~Logger()
{
    append_end();
    LogStream::Buffer &buf = m_stream.get_buffer();
    g_output_fun(buf.data(), buf.len());
    g_flush_fun();
}

Logger::Logger(const char *filename, int num, int level) : m_filename(filename),
                                                           m_line(num), m_level(level)
{
    // 先把日期，时间，和日志等级加上
    append_prefix();
}

void Logger::append_prefix()
{
    append_time();
    append_level();
    // todo: 每条日志最前面需要加的都加在这里
}

void Logger::append_time()
{
    char time_buf[40];
    struct timeval tv;
    gettimeofday(&tv, nullptr);

    int64_t microsecondsSinceEpoch = tv.tv_sec * MicrosecondsPerSecond + tv.tv_usec;
    auto seconds = static_cast<time_t>(microsecondsSinceEpoch / MicrosecondsPerSecond);
    int microseconds = static_cast<int>(microsecondsSinceEpoch % MicrosecondsPerSecond);
    struct tm tm_time;
    gmtime_r(&seconds, &tm_time);
    // todo: 秒之前的部分 1 s 构造一次

    snprintf(time_buf, sizeof(time_buf), "%4d%02d%02d %02d:%02d:%02d",
             tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
             tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);

    m_stream.append(time_buf);
}

const char *level_str[Logger::NUM_LEVEL] = {
    "[DEBUG] ",
    "[INFO]  ",
    "[WARN]  ",
    "[ERROR] "};

void Logger::append_level()
{
    m_stream.append(level_str[m_level]);
}

void Logger::append_end()
{
    // 加上文件名和行号
    m_stream << " ----- " << m_filename << " : " << m_line << "\n";
}

void Logger::default_output_fun(const char *data, size_t len)
{
    fwrite(data, 1, len, stdout);
}

void Logger::default_flush_fun()
{
    fflush(stdout);
}
