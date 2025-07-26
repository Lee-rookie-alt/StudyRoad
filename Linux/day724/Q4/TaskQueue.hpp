#ifndef __TASKQUEUE_H 
#define __TASKQUEUE_H
#include "MutexLock.hpp"
#include "Condition.hpp"
#include <unistd.h>
#include <list>
class TaskQueue{
public:
    TaskQueue(size_t);
    ~TaskQueue();
    void push(int);
    int pop();
    int size() const { return _list.size(); }
    bool empty() const;
    bool full() const;
private:
    std::list<int> _list;
    int _listsize;
    MutexLock _mutex;
    Condition _notfull;
    Condition _notempty;
};
#endif

