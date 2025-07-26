#include "Thread.hpp"
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <memory>
using std::unique_ptr;
class MyThread
{
public:
    MyThread(){}
    ~MyThread(){}
    void process(){
        srand(time(nullptr));
        while(1){
            int data=rand()%100;
            printf("process a number is %d\n",data);
            sleep(1);
        }
    }
private:
};
void test(){
    MyThread task;
    unique_ptr<Thread> th1(new Thread(std::bind(&MyThread::process,&task)));
    th1->start();
    th1->join();
    printf("main thread is exiting!\n");
}   
int main()
{
    test();
    return 0;
}

