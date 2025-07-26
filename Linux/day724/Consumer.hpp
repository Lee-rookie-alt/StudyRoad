#ifndef __CONSUMER_HPP 
#define __CONSUMER_HPP
#include "Thread.hpp"
#include "TaskQueue.hpp"
#include <stdio.h>
#include <unistd.h>
class Consumer:public Thread{
public:
    Consumer(TaskQueue &que)
    :_taskque(que)
    {}
    ~Consumer(){}
private:
    void run() override{
        int cnt=10;
        while(cnt-->0){
            int data=_taskque.pop();
            printf("Consumer consume a data is %d\n",data);
            sleep(1);
        }
    }
    TaskQueue &_taskque;
};
#endif

