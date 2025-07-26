#include "ThreadPool.h"
#include "Task.h"
#include <iostream>
#include <chrono>
using std::cout;
using std::endl;
ThreadPool::ThreadPool(size_t threadnum,size_t quesize)
:_threadNum(threadnum)
,_queSize(quesize)
    ,_threads()
    ,_taskQue(quesize)
    ,isExit(false)
{}
ThreadPool::~ThreadPool(){}
void ThreadPool::start(){
    for(size_t idx=0;idx<_threadNum;idx++){
        _threads.push_back(thread(&ThreadPool::doTask,this));
    }
}
void ThreadPool::stop(){
    while(!_taskQue.empty()){
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    isExit=true;
    _taskQue.wakeup();
    for(auto &ele : _threads){
        ele.join();
    }
    cout<<"ThreadPoll  end!"<<endl;
}
Task* ThreadPool::getTask(){
    return _taskQue.pop();
}
void ThreadPool::doTask(){
    while(!isExit){
        Task *p=getTask();
        if(p){
        p->process();
        }
        else{
        cout<<"task is null!"<<endl;
        }
    }
}
void ThreadPool::addTask(Task * p){
    if(p){
        _taskQue.push(p);
    }else
    {
        cout<<"task push error! because task *p is null"<<endl;
    }
}
