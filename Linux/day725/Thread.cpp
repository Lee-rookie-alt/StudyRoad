#include "Thread.hpp"
#include <iostream>
Thread::Thread()
    :_pthid(0)
     ,_isRunning(false)
{
}
Thread::~Thread(){}
void Thread::start(){
    if(!_isRunning){
        int ret=pthread_create(&_pthid,nullptr,start_routine,this);
        if(ret !=0){
            printf("pthread_create failed!\n");
            return ;
        }
        _isRunning=true;
    }
}
void Thread::join(){
    printf("join!!!!\n");
    if(_isRunning){
        pthread_join(_pthid,nullptr);
        _isRunning=false;
    }
}
void *Thread::start_routine(void *  arg){
    Thread * pthread = static_cast<Thread*> (arg);
    if(pthread){
        pthread->run();
    }
    return nullptr;
}
