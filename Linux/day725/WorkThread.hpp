#ifndef __WORKTHREAD_HPP
#define __WORKTHREAD_HPP
#include "Thread.hpp"
#include "ThreadPool.hpp"

class WorkThread : public Thread
{
public:
    WorkThread(ThreadPool & pool)
    :_threadpool(pool)
    {}
private:
    void run() override{
        _threadpool.doTask();
    }
private:
    ThreadPool &_threadpool;
};
#endif