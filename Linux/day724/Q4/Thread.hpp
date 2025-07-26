#ifndef __THREAD_HPP 
#define __THREAD_HPP
#include <pthread.h>
#include <unistd.h>
class Thread{
public:
    Thread();
    ~Thread();
    void start();
    void join();
private:
     virtual void run()=0;
    static void * start_routine(void *);
private:
    pthread_t _pthid;
    bool _isRunning;
};
#endif

