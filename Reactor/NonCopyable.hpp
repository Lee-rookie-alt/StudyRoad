#ifndef __NONCOPYABLE_HPP
#define __NONCOPYABLE_HPP

class NonCopyable{
protected:
    NonCopyable()=default;
    ~NonCopyable()=default;

    NonCopyable(const NonCopyable &) =delete;
    NonCopyable &operator=(const NonCopyable &) =delete;
};
#endif

