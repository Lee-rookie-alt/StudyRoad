#ifndef __CONDITION_H 
#define __CONDITION_H
#include <pthread.h>
#include <unistd.h>
#include <memory>
class MutexLock;
class Condition{
public:
    Condition(MutexLock & m);
    ~Condition();
    void wait();
    void notify();
    void notify_all();

private:
    pthread_cond_t _cond;
    MutexLock & _mutex;
};
#endif

