#ifndef __TASKQUEUE_H 
#define __TASKQUEUE_H
#include "MutexLock.hpp"
#include "Condition.hpp"
#include <unistd.h>
#include <queue>
class TaskQueue{
public:
    TaskQueue(size_t);
    ~TaskQueue();
    void push(int);
    int pop();
    int size() const { return _que.size(); }
    bool empty() const;
    bool full() const;
private:
    std::queue<int> _que;
    int _quesize;
    MutexLock _mutex;
    Condition _notfull;
    Condition _notempty;
};
#endif

