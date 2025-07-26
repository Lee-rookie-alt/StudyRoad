#include "TaskQueue.hpp"
#include <iostream>
TaskQueue::TaskQueue(size_t a)
    :_que()
     ,_quesize(a)
     ,_mutex()
     ,_notfull(_mutex)
     ,_notempty(_mutex)
     ,_isflag(false)
{
    
}
TaskQueue::~TaskQueue(){}
bool TaskQueue::empty() const{
    if(size()==0){
        return true;
    }
    return false;
}
bool TaskQueue::full() const{
    if(size()==_quesize){
        return true;
    }
    return false;
}
void TaskQueue::push(Task* a){
    MutexLockGuard autolock(_mutex);
    while(full()){//队列满了
        _notfull.wait();
    }
    _que.push(a);
    _notempty.notify();
}
void TaskQueue::start(){
    _isflag=true;
}
void TaskQueue::end(){
    _isflag=false;
}
void TaskQueue::notify_all(){
    _notempty.notify_all();
}
Task * TaskQueue::pop(){
    MutexLockGuard autolock(_mutex);
    while(empty() && _isflag){//队列是空的
        _notempty.wait();
    }
    if(_isflag){
    Task* temp=_que.front();
    _que.pop();
    _notfull.notify();
    return temp;
    }
    else return nullptr;
    
}

