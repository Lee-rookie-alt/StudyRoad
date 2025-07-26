#ifndef __MUTEXLOCK_H
#define __MUTEXLOCK_H
#include "noncopyable.hpp"
#include <pthread.h>
class MutexLock : Noncopyable{
public:
    MutexLock();
    ~MutexLock();
    void lock();
    void unlock();
    pthread_mutex_t *getMutexPtr(){ return &_mutex; }
private:
    pthread_mutex_t _mutex;
};
class MutexLockGuard{
public:
    MutexLockGuard(MutexLock &m)
    :_mutex(m)
    {
        _mutex.lock();
    }
    ~MutexLockGuard(){
        _mutex.unlock();
    }
private:
    MutexLock &_mutex;
};
#endif

