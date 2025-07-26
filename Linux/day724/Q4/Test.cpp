#include "Producer.hpp"
#include "Consumer.hpp"
#include "TaskQueue.hpp"
#include <memory>
#include <time.h>
#include <stdlib.h>
#include <string>
#include <iostream>
using std::string;
using std::unique_ptr;
int main(){
    TaskQueue taskque(10);
    srand(time(nullptr));
    for(int i=0;i<8;i++){
        int data=rand()%100;
        taskque.push(data);
    }
    string p1("producer1");
    string p2("producer2");
    string p3("producer3");
    unique_ptr<Thread> producer1(new Producer(taskque,p1));
    unique_ptr<Thread> producer2(new Producer(taskque,p2));
    unique_ptr<Thread> producer3(new Producer(taskque,p3));
    string c1("Consumer1");
    string c2("Consumer2");
    unique_ptr<Thread> consumer1(new Consumer(taskque,c1));
    unique_ptr<Thread> consumer2(new Consumer(taskque,c2));
    producer1->start();
    producer2->start();
    producer3->start();
    consumer1->start();
    consumer2->start();
    producer1->join();
    producer2->join();
    producer3->join();
    consumer1->join();
    consumer2->join();
    printf("main thread is exiting!\n");
    return 0;
}
