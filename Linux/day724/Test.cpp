#include "Producer.hpp"
#include "Consumer.hpp"
#include "TaskQueue.hpp"
#include <memory>
using std::unique_ptr;
int main(){
    TaskQueue taskque(5);
    unique_ptr<Thread> producer(new Producer(taskque));
    unique_ptr<Thread> consumer(new Consumer(taskque));
    producer->start();
    consumer->start();
    producer->join();
    consumer->join();
    printf("main thread is exiting!\n");
    return 0;
}
