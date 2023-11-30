/*
 * @Author: liukboy wowkliu@163.com
 * @Date: 2023-10-31 12:21:13
 * @Description: 日志文件类.主要功能：创建文件，把字符串写到文件。
 *
 * Copyright (c) 2023 by liukboy, All Rights Reserved.
 */
#pragma once
#include <memory>
#include <thread>
#include <string>
#include <mutex>

using namespace std;
class LogFile
{
public:
    LogFile(const string &filename, size_t rollsize);
    ~LogFile(); // 因为我们用了pImpl方式编程，必须在cpp文件中实现析构函数，要不然invalid application of ‘sizeof’ to incomplete type ‘LogFile::File’

    const string get_log_file_name(const std::string &basename, time_t *now);

    void rollfile();
    void append(const char *line, size_t len);
    void flush();

private:
    string m_filename;
    size_t m_rollsize;

    mutex m_mutex;
    class File;
    unique_ptr<File> m_fileptr;

    const static int g_check_time_roll = 1024;      // 表示每写入 g_check_time_roll 次就flush一次文件缓冲区。这个值先硬编码，可以通过配置或者构造函数传进来。
    const static int g_refresh_time = 60 * 60 * 24; // 每天凌晨0点会新建文件
    const static int g_flush_interval = 3;          // 3s刷新一下文件
    time_t m_last_log_start_second;                 // 上次日志是哪天
    // time_t lastRoll_;
    time_t m_last_flush; // 上次刷新时间
    int m_cnt;           // 写入日志次数
};