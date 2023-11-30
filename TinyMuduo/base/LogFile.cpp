#include "LogFile.h"

#include <cstdio>
#include <unistd.h>
#include <errno.h>

/*
File的操作都在这个类中实现，实现细节隐藏，也使得LogFile更加简洁
*/
class LogFile::File
{
public:
    explicit File(const string &filename) : m_file(::fopen(filename.c_str(), "ae")), m_write_bytes(0)
    {
        ::setbuffer(m_file, buf, sizeof(buf));
    }
    void append(const char *line, size_t len);

    ~File()
    {
        ::fclose(m_file);
    }

    void flush()
    {
        ::fflush(m_file);
    }
    size_t get_write_bytes() const { return m_write_bytes; }

private:
    size_t write(const char *line, size_t len) const
    {
        ::fwrite_unlocked(line, 1, len, m_file);
    }
    FILE *m_file;
    char buf[64 * 1024];  // 这个值是照抄muduo
    size_t m_write_bytes; // 统计文件写了多少字节
};

void LogFile::File::append(const char *line, size_t len)
{
    size_t written = 0;
    while (written != len)
    {
        size_t n = write(line + written, len - written);
        if (n == 0)
        {
            int err = ferror(m_file);
            if (err)
            {
                fprintf(stderr, "LogFile::File::append() failed\n");
            }
            break;
        }
        written += n;
    }
    m_write_bytes += len;
}

LogFile::LogFile(const string &filename, size_t rollsize) : m_filename(move(filename)),
                                                            m_rollsize(rollsize)
{
    rollfile();
}

LogFile::~LogFile() = default;

const string LogFile::get_log_file_name(const std::string &basename, time_t *now)
{
    std::string filename;
    filename.reserve(basename.size() + 64);
    filename = basename;

    char timebuf[32];
    char pidbuf[32];
    struct tm tm;
    *now = time(nullptr);
    localtime_r(now, &tm);
    strftime(timebuf, sizeof(timebuf), ".%Y%m%d-%H%M%S.", &tm);
    filename += timebuf;
    snprintf(pidbuf, sizeof(pidbuf), "%d", ::getpid());
    filename += pidbuf;
    filename += ".log";

    return filename;
}

void LogFile::rollfile()
{
    time_t now = 0;
    const std::string filename = get_log_file_name(m_filename, &now);
    time_t start = now / g_refresh_time * g_refresh_time;
    m_last_log_start_second = start;
    m_last_flush = start;
    m_fileptr = make_unique<File>(filename);
}

void LogFile::append(const char *line, size_t len)
{
    lock_guard<mutex> lg(m_mutex);
    m_fileptr->append(line, len);

    if (m_fileptr->get_write_bytes() > m_rollsize)
    {
        // 如果文件写进的字符已经超过上限，那么新建一个文件
        m_fileptr->flush();
        rollfile();
    }
    else
    {
        m_cnt++;
        if (m_cnt >= g_check_time_roll)
        {
            // 已经连续写入日志 g_check_time_roll次
            m_cnt = 0;
            time_t now = ::time(nullptr);
            time_t this_period_ = now / g_refresh_time * g_refresh_time;
            if (this_period_ != m_last_log_start_second)
            {
                // 已经跨天了，新建文件
                rollfile();
            }
            else if (now - m_last_flush > g_flush_interval)
            {
                m_last_flush = now;
                m_fileptr->flush();
            }
        }
    }
}

void LogFile::flush()
{
    lock_guard<mutex> lg(m_mutex);
    m_fileptr->flush();
}
