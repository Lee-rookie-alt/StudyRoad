#include "TaskQueue.hpp"
#include <iostream>
TaskQueue::TaskQueue(size_t a)
    :_list()
     ,_listsize(a)
     ,_mutex()
     ,_notfull(_mutex)
     ,_notempty(_mutex)
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
    if(size()==_listsize){
        return true;
    }
    return false;
}
void TaskQueue::push(int a){
    MutexLockGuard autolock(_mutex);
    while(full()){//队列满了
        _notfull.wait();
    }
    _list.push_back(a);
    _notempty.notify();
}
int TaskQueue::pop(){
    MutexLockGuard autolock(_mutex);
    while(empty()){//队列是空的
        _notempty.wait();
    }
    int temp=_list.front();
    _list.pop_front();
    _notfull.notify();
    return temp;
}

