#ifndef __PRODUCER_HPP 
#define __PRODUCER_HPP
#include "Thread.hpp"
#include "TaskQueue.hpp"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
class Producer:public Thread{
public:
    Producer(TaskQueue &que)
    :_taskque(que)
    {}
    ~Producer(){}
private:
    void run() override{
        int cnt=10;
        srand(time(nullptr));
        while(cnt-->0){
            int data=rand()%100;
            _taskque.push(data);
            printf("Producer produce a data is %d\n",data);
        }
    }
private:
    TaskQueue &_taskque;
};
#endif

