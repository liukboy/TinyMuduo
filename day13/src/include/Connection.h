#pragma once
#include <functional>
#include "Macros.h"
/*
连接类，当有socket连接，就新建这么一个类
*/
class EventLoop;
class Channel;
class Socket;
class Buffer;
class Connection {
 public:
  Connection(EventLoop *loop, Socket *sock);
  ~Connection();

  DISALLOW_COPY_AND_MOVE(Connection);

  void Echo(int sockfd);
  void SetDeleteConnectionCallback(std::function<void(int)> const &callback);
  void Send(int sockfd);

 private:
  EventLoop *loop_;
  Socket *sock_;
  Channel *channel_;
  std::function<void(int)> delete_connectioin_callback_;
  Buffer *read_buffer_;
};
