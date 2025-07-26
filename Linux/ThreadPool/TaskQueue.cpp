#include "TaskQueue.h"
TaskQueue::TaskQueue(size_t quesize)
:_queSize(quesize)
    ,_que()
    ,_mutex()
    ,_notFull()
    ,_notEmpty()
    ,_flag(false)
{}
TaskQueue::~TaskQueue(){}
using std::unique_lock;
void TaskQueue::push(Task * p){
    unique_lock<mutex> ul(_mutex);
    while(full()){
        _notFull.wait(ul);
    }
    _que.push(p);
    _notEmpty.notify_one();
}
Task * TaskQueue::pop(){
    unique_lock<mutex> ul(_mutex);
    while(empty() && !_flag){
        _notEmpty.wait(ul);
    }
    if(!_flag)
    {
        Task *temp=_que.front();
        _que.pop();
        _notFull.notify_one();
        return temp;
    }else{
        return nullptr;
    }
}
bool TaskQueue::full(){
    return _que.size()==_queSize;
}
bool TaskQueue::empty(){
    return _que.size()==0;
}
void TaskQueue::wakeup(){
    _flag=true;
    _notEmpty.notify_all();
}
