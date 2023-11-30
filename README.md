# Tinymuduo
### 简介
本仓库是改写陈硕大佬的muduo网络库。muduo基于事件驱动和事件回调的epoll+线程池面向对象编程，采用Multi-Reactor架构以及One Loop Per Thread设计思想的多线程C++网络库。Tinymuduo重写了muduo核心组件，去除了对boost库的依赖，使用C++11重构，使用互斥锁以及智能指针管理对象。重要组件包括Channel、Poller、EventLoop、TcpServer等，同时还实现了Buffer缓冲区。

### 目录说明
day*:表示从最简单的创建socket连接，每一天都增加一些功能，一步一步完善功能
TinyMuduo:最终版本

### 技术特点
* 完全去掉了Muduo库中的Boost依赖，全部使用C++11新语法，如智能指针、function函数对象、lambda表达式等
* 没有单独封装Thread，使用C++11引入的std::thread搭配lambda表达式实现工作线程，没有直接使用pthread库
* 只实现了epoll这一个IO-Multiplexing,未实现poll
* 整个项目采用Cmake编译，同时使用shell脚本实用一键编译
### 使用说明
* 环境：Linux系统，g++需要支持C++11及以上标准
* 编译安装：运行Tinymuduo目录下的autobuild.sh文件
```
sudo sh autobuild.sh
```
autobuild脚本会将整个项目编译,可执行文件在build/bin目录下

### 项目梳理
#### 第一模块：基本类
* Uncopyable: 不可复制类
* Logger：日志类
* Timestamp：时间类
* InetAddress：封装 socket 地址
#### 第二模块：Channel、Poller、EventLoop
* Channel：打包 fd 和感兴趣的 events
* Poller：epoll、poll 的基类
* EPollPoller：封装 epoll 的操作
* CurrentThread：获取当前线程 tid，每个 channel 要在自己的 eventloop 线程上进行处理
* Eventloop：事件轮询，连接 poller 和 channel
* Thread：底层线程
* EventLoopThread：one loop per thread，绑定一个 loop 和一个 thread
* EventLoopThreadPool：线程池，有 main loop 和 sub loop
#### 第三模块：Acceptor、TcpServer、TcpConnection
* Socket：封装socket、bind、listen、accept等操作
* InetAddress：封装socket地址
* Acceptor：处理accept，监听新用户的连接，打包成channel，分发给subloop
* TcpServer：集大成者，负责协调各个类之间的关系，实现多线程、多连接的网络编程
* Buffer：实现缓冲区
* TcpConnection：实现建立和客户端的新连接，处理连接上的数据发送和接收

### 技术亮点
1. EventLoop中使用了eventfd来调用wakeup()，让mainloop唤醒subloop的epoll_wait阻塞
2. 在EventLoop中注册回调cb至pendingFunctors_，并在doPendingFunctors中通过swap()的方式，快速换出注册的回调，只在swap()时加锁，减少代码临界区长度，提升效率。（若不通过swap()的方式去处理，而是加锁执行pendingFunctors中的回调，然后解锁，会出现什么问题呢？1. 临界区过大，锁降低了服务器响应效率 2. 若执行的回调中执行queueInLoop需要抢占锁时，会发生死锁）
3. Logger可以设置日志等级，调试代码时可以开启DEBUG打印日志；若启动服务器，由于日志会影响服务器性能，可适当关闭DEBUG相关日志输出
4. muduo采用Reactor模型和多线程结合的方式，实现了高并发非阻塞网络库

### TODO
1. 定时器模块
2. 实现功能性配套程序，比如HTTP服务器
3. 单元测试


### 感谢
- 《Linux高性能服务器编程》
- 《Linux多线程服务端编程：使用muduo C++网络库》
- https://github.com/chenshuo/muduo