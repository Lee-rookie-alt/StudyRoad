#ifndef __THREADPOOL_H
#define __THREADPOOL_H
#include <thread>
#include <vector>
#include "TaskQueue.h"
#include "Task.h"
using std::vector;
using std::thread;
class ThreadPool{
public:
    ThreadPool(size_t threadnum,size_t quesize);
    ~ThreadPool();
    void start();
    void stop();
    void addTask(Task *);
private:
    size_t _threadNum;
    size_t _queSize;
    vector<thread> _threads;
    TaskQueue _taskQue;
    bool isExit;
    Task * getTask();
    void doTask();
};
#endif

