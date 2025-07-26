#include "ThreadPool.hpp"
#include "WorkThread.hpp"
#include "stdio.h"
ThreadPool::ThreadPool(int threadsize,int quesize)
:_threads()
,_threadsize(threadsize)
,_quesize(quesize)
,_isExit(false)
,_que(quesize)
{
    _threads.reserve(threadsize);
}
ThreadPool::~ThreadPool(){}
void ThreadPool::start(){
    for(int i=0;i<_threadsize;i++){
        unique_ptr<Thread> up(new WorkThread(*this));
        _threads.push_back(std::move(up));
    }
    for(auto &ele:_threads){
        ele->start();//将其中的线程都统一start
    }
    _que.start();
}
void ThreadPool::stop(){
    while(!_que.empty()){
        sleep(1);
    }
    _isExit=true;
    printf("threads joining\n");
    int i=0;
    _que.end();
    _que.notify_all();
    for(auto &ele:_threads){
        printf("stop thread join %d time\n",i+1);
        ele->join();
        
        i++;
    }
    printf("stop the threadPool\n");
}
void ThreadPool::addTask(Elemtype e){
    if(e){
    _que.push(e);
    }
}
void ThreadPool::doTask(){
    while (!_isExit)
    {
        Task * task=_que.pop();
        if(task){
            task->process();
        }else {
            printf("task is null!\n");
        }
    }
}