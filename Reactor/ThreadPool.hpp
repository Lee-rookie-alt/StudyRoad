#ifndef __THREADPOOL_HPP
#define __THREADPOOL_HPP
#include "Thread.hpp"
#include "TaskQueue.hpp"
#include "Task.hpp"
//#include "WorkThread.hpp"

#include <vector>
#include <memory>
#include <unistd.h>
using std::vector;
using std::unique_ptr;
class ThreadPool{
public:
    ThreadPool(int threadsize,int quesize);
    ~ThreadPool();
    void stop();
    void start();
    void addTask(Task &&);
private:
    void doTask();//这个不是接口
private:
    vector<unique_ptr<Thread>> _threads;
    int _threadsize;
    int _quesize;
    bool _isExit;
    TaskQueue _que;
};

#endif
