#include <ctime>
#include <iostream>
#include <memory>
#include "Task.h"
#include "TaskQueue.h"
#include "ThreadPool.h"
using std::cout;
using std::endl;
using std::unique_ptr;
class MyTask
:public Task
{
public:
    virtual void process(){
        ::srand(::clock());
        int number=::rand()%100;
        cout<<">>Mytask process the number is "<<number<<endl;
    }
};
void test(){
    unique_ptr<Task> ptask(new MyTask());
    ThreadPool pool(6,8);
    pool.start();
    int cnt=15;
    while(cnt--){
        pool.addTask(ptask.get());
        cout<<"cnt : "<<cnt<<endl;
    }
    pool.stop();
}   
int main()
{
    test();
    return 0;
}

