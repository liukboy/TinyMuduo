
#pragma once
#include "Macros.h"

#include <functional>

class Epoll;
class Channel;
class EventLoop {
 public:
  EventLoop();
  ~EventLoop();

  DISALLOW_COPY_AND_MOVE(EventLoop);

  void Loop();
  void UpdateChannel(Channel *ch);

 private:
  Epoll *epoll_{nullptr};
  bool quit_{false};
};
