#ifndef __TASKQUEUE_H 
#define __TASKQUEUE_H
#include "MutexLock.hpp"
#include "Condition.hpp"
#include "Thread.hpp"
#include "Task.hpp"
#include <unistd.h>
#include <queue>
using ElemType=Task;
class TaskQueue{
public:
    TaskQueue(size_t);
    ~TaskQueue();
    void push(ElemType);
    ElemType pop();
    int size() const { return _que.size(); }
    bool empty() const;
    bool full() const;
    void start();
    void end();
    void notify_all();
private:
    std::queue<ElemType> _que;
    int _quesize;
    MutexLock _mutex;
    Condition _notfull;
    Condition _notempty;
    bool _isflag;
};
#endif

