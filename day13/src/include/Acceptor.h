
#pragma once
#include <stdio.h>
#include "Macros.h"

#include <functional>

class EventLoop;
class Socket;
class Channel;
class Acceptor {
 public:
  explicit Acceptor(EventLoop *loop);
  ~Acceptor();

  DISALLOW_COPY_AND_MOVE(Acceptor);

  void AcceptConnection();
  void SetNewConnectionCallback(std::function<void(Socket *)> const &callback);

 private:
  EventLoop *loop_;
  Socket *sock_;
  Channel *channel_;
  std::function<void(Socket *)> new_connection_callback_;
};
