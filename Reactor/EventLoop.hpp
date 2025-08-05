#ifndef __EVENTLOOP_HPP
#define __EVENTLOOP_HPP
#include "TcpConnection.hpp"
#include "Acceptor.hpp"
#include "MutexLock.hpp"
#include <time.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <sys/timerfd.h>
#include <vector>
#include <map>
#include <memory>
#include <functional>
#include <mutex>
using std::vector;
using std::map;
using std::shared_ptr;
using std::mutex;
class TcpConnection;
class Acceptor;
using TcpConnectionPtr = shared_ptr<TcpConnection>;
using TcpConnectionCallback = std::function<void(TcpConnectionPtr)>;
using Functor=std::function<void()>;


class EventLoop{
public:
    EventLoop(Acceptor &accept);
    ~EventLoop();
    void Loop();
    void unLoop();
    void runInLoop(Functor && cb);
    void setNewConnectionCallback(TcpConnectionCallback &&cb);
    void setMessageCallback(TcpConnectionCallback &&cb);
    void setCloseCallback(TcpConnectionCallback &&cb);
    void handleNewConnection();
    void handleMessage(int fd);
    void HandleRead();
    void Wakeup();
    void doPengdingFunctors();//对vector中的Task直接调用?
private:
    void waitEpollFd();
    int CreateEpollFd();
    int CreateEventFd();
    int CreateTimefd();
    void addEpollReadFd(int fd);
    void delEpoolReadFd(int fd);
    void BroadCast(const string &msg);
    void initTimefd();
    void TimeCheck();
    
private:
    int _epfd;
    int _evtfd;//eventFd 可以起到通知作用
    int _timefd;
    vector<struct epoll_event> _eventList;
    bool _isLooping;
    Acceptor & _accept;
    map<int,TcpConnectionPtr> _conns;
    map<int,time_t>           _timecheck;
    TcpConnectionCallback _onNewConnection;
    TcpConnectionCallback _onMessage;
    TcpConnectionCallback _onClose;
    vector<Functor> _pendings;//存放Task的地方？
    MutexLock _mutex;//互斥锁？
};
#endif