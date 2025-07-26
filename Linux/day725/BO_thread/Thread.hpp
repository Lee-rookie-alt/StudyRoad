#ifndef __THREAD_HPP 
#define __THREAD_HPP
#include "noncopyable.hpp"
#include <pthread.h>
#include <unistd.h>

#include <functional>

using ThreadCallback=std::function<void ()>;
class Thread:Noncopyable
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

