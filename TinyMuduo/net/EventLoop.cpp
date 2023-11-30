#include "EventLoop.h"
#include "Logger.h"
#include "Poller.h"
#include "Channel.h"

#include <sys/eventfd.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <memory>

// 防止一个线程创建多个EventLoop   thread_local
__thread EventLoop *t_loopInThisThread = nullptr;

// 定义默认的Poller IO复用接口的超时时间
const int kPollTimeMs = 10000;

// 创建wakeupfd，用来notify唤醒subReactor处理新来的channel
int createEventfd()
{
    int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (evtfd < 0)
    {
        LOG_ERROR << "eventfd error: " << errno;
    }
    return evtfd;
}

EventLoop::EventLoop()
    : m_looping(false),
      m_quit(false),
      m_callingPendingFunctors(false),
      m_threadId(CurrentThread::tid()),
      m_poller(Poller::newDefaultPoller(this)),
      m_wakeupFd(createEventfd()),
      m_wakeupChannel(new Channel(this, m_wakeupFd))
{
    LOG_DEBUG << "EventLoop created in thread " << m_threadId;
    if (t_loopInThisThread)
    {
        LOG_ERROR << "Another EventLoop" << t_loopInThisThread << "exists in this thread " << m_threadId;
    }
    else
    {
        t_loopInThisThread = this;
    }

    // 设置wakeupfd的事件类型以及发生事件后的回调操作
    m_wakeupChannel->setReadCallback(std::bind(&EventLoop::handleRead, this));
    // 每一个eventloop都将监听wakeupchannel的EPOLLIN读事件了
    m_wakeupChannel->enableReading();
}

EventLoop::~EventLoop()
{
    m_wakeupChannel->disableAll();
    m_wakeupChannel->remove();
    ::close(m_wakeupFd);
    t_loopInThisThread = nullptr;
}

// 开启事件循环
void EventLoop::loop()
{
    m_looping = true;
    m_quit = false;

    LOG_INFO << "EventLoop " << this << "start looping ";

    while (!m_quit)
    {
        m_activeChannels.clear();
        // 监听两类fd   一种是client的fd，一种wakeupfd
        m_pollReturnTime = m_poller->poll(kPollTimeMs, &m_activeChannels);
        for (Channel *channel : m_activeChannels)
        {
            // Poller监听哪些channel发生事件了，然后上报给EventLoop，通知channel处理相应的事件
            channel->handleEvent(m_pollReturnTime);
        }
        // 执行当前EventLoop事件循环需要处理的回调操作
        /**
         * IO线程 mainLoop accept fd《=channel subloop
         * mainLoop 事先注册一个回调cb（需要subloop来执行）    wakeup subloop后，执行下面的方法，执行之前mainloop注册的cb操作
         */
        doPendingFunctors();
    }

    LOG_INFO << "EventLoop" << this << " stop looping.";
    m_looping = false;
}

// 退出事件循环  1.loop在自己的线程中调用quit  2.在非loop的线程中，调用loop的quit
/**
 *              mainLoop
 *
 *                                             no ==================== 生产者-消费者的线程安全的队列
 *
 *  subLoop1     subLoop2     subLoop3
 */
void EventLoop::quit()
{
    m_quit = true;

    // 如果是在其它线程中，调用的quit   在一个subloop(woker)中，调用了mainLoop(IO)的quit
    if (!isInLoopThread())
    {
        wakeup();
    }
}

// 在当前loop中执行cb
void EventLoop::runInLoop(Functor cb)
{
    if (isInLoopThread()) // 在当前的loop线程中，执行cb
    {
        cb();
    }
    else // 在非当前loop线程中执行cb , 就需要唤醒loop所在线程，执行cb
    {
        queueInLoop(cb);
    }
}
// 把cb放入队列中，唤醒loop所在的线程，执行cb
void EventLoop::queueInLoop(Functor cb)
{
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_pendingFunctors.emplace_back(cb);
    }

    // 唤醒相应的，需要执行上面回调操作的loop的线程了
    // || callingPendingFunctors_的意思是：当前loop正在执行回调，但是loop又有了新的回调
    if (!isInLoopThread() || m_callingPendingFunctors)
    {
        wakeup(); // 唤醒loop所在线程
    }
}

void EventLoop::handleRead()
{
    uint64_t one = 1;
    ssize_t n = read(m_wakeupFd, &one, sizeof one);
    if (n != sizeof one)
    {
        LOG_ERROR << "EventLoop::handleRead() reads " << n << " bytes instead of 8";
    }
}

// 用来唤醒loop所在的线程的  向wakeupfd_写一个数据，wakeupChannel就发生读事件，当前loop线程就会被唤醒
void EventLoop::wakeup()
{
    uint64_t one = 1;
    ssize_t n = write(m_wakeupFd, &one, sizeof one);
    if (n != sizeof one)
    {
        LOG_ERROR << "EventLoop::wakeup() writes" << n << "bytes instead of 8 ";
    }
}

// EventLoop的方法 =》 Poller的方法
void EventLoop::updateChannel(Channel *channel)
{
    m_poller->updateChannel(channel);
}

void EventLoop::removeChannel(Channel *channel)
{
    m_poller->removeChannel(channel);
}

bool EventLoop::hasChannel(Channel *channel)
{
    return m_poller->hasChannel(channel);
}

void EventLoop::doPendingFunctors() // 执行回调
{
    std::vector<Functor> functors;
    m_callingPendingFunctors = true;

    {
        std::unique_lock<std::mutex> lock(m_mutex);
        functors.swap(m_pendingFunctors);
    }

    for (const Functor &functor : functors)
    {
        functor(); // 执行当前loop需要执行的回调操作
    }

    m_callingPendingFunctors = false;
}