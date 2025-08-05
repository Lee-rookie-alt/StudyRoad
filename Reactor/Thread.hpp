#ifndef __THREAD_HPP 
#define __THREAD_HPP
#include "NonCopyable.hpp"
#include <pthread.h>
#include <unistd.h>
#include <functional>
using std::function;
using ThreadCallback=function<void()>;
class Thread:NonCopyable
{
public:
    Thread(ThreadCallback &&cb);
    ~Thread();
    void start();
    void join();
private:
    static void * start_routine(void *);
private:
    pthread_t _pthid;
    bool _isRunning;
    ThreadCallback _cb;
};
#endif

