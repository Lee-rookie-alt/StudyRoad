#ifndef __CONSUMER_HPP 
#define __CONSUMER_HPP
#include "Thread.hpp"
#include "TaskQueue.hpp"
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <string>
using std::string;
class Consumer:public Thread{
public:
    Consumer(TaskQueue &que,string name)
    :_taskque(que)
     ,_name(name)
    {}
    ~Consumer(){}
private:
    void run() override{
        sleep(5);
        while(1){
            int data=_taskque.pop();
            std::cout<<_name<<" conseme a data is "<<data<<std::endl;
            sleep(1);
        }
    }
private:
    TaskQueue &_taskque;
    string _name;
};
#endif

