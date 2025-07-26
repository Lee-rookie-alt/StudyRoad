#ifndef __TASK_HPP
#define __TASK_HPP
class Task{
public:
    Task(){}
    ~Task(){}
    virtual void process()=0;
};
#endif