#include "ThreadPool.hpp"
#include "TaskQueue.hpp"
#include "Task.hpp"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include <memory>
using std::unique_ptr;
class MyTask {
public:
    MyTask(){}
    ~MyTask(){}
    void process() {
        srand(time(nullptr));
        int data=rand()%100;
        printf("process a data is %d \n",data);
        sleep(1);
    }
};
int main(){
    MyTask t1;
    ThreadPool tpool(3,8);
    tpool.start();
    int cnt=10;
    while(cnt-->0){
        tpool.addTask(std::bind(&MyTask::process,&t1));
        printf("cnt: %d \n",cnt);
    }
    tpool.stop();
    printf("main thread is Exiting!\n");
    return 0;
}
