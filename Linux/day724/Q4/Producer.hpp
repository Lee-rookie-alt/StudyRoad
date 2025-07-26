#ifndef __PRODUCER_HPP 
#define __PRODUCER_HPP
#include "Thread.hpp"
#include "TaskQueue.hpp"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <iostream>
using std::string;
class Producer:public Thread{
public:
    Producer(TaskQueue &que, string &name)
    :_taskque(que)
     ,_name(name)
    {}
    ~Producer(){}
private:
    void run() override{
        srand(time(nullptr));
        while(1){
            int data=rand()%100;
            _taskque.push(data);
            std::cout<<_name<<" produce a data is "<<data<<std::endl;
            sleep(3);
        }
    }
private:
    TaskQueue &_taskque;
    string &_name;
};
#endif

