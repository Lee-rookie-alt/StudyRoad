#ifndef __NONCOPYABLE_HPP
#define __NONCOPYABLE_HPP
class Noncopyable{
protected:
    Noncopyable(){}
    ~Noncopyable(){}
    Noncopyable(const Noncopyable &) =delete;
    Noncopyable &operator=(const Noncopyable &) =delete;
};
#endif

