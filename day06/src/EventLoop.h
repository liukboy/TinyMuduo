#include "Epoll.h"
#include "Channel.h"

class EventLoop
{
public:
    EventLoop();
    void updateChannel(Channel *);

    void loop();

private:
    Epoll *ep;
    bool quit;
};