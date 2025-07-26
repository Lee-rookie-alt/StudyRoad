#ifndef __TASKQUEUE_H 
#define __TASKQUEUE_H
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>
#include "Task.h"
using std::queue;
using std::size_t;
using std::mutex;
using std::condition_variable;
class TaskQueue{
public:
    TaskQueue(size_t quesize);
    ~TaskQueue();
    void push(Task *);
    Task * pop();
    bool full();
    bool empty();
    void wakeup();
private:
    size_t _queSize;
    queue<Task *> _que;
    mutex _mutex;
    condition_variable _notFull;
    condition_variable _notEmpty;
    bool _flag;
};
#endif

