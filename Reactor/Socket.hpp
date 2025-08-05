#ifndef __SOCKET_HPP
#define __SOCKET_HPP
#include "NonCopyable.hpp" 
#include <netHeader.h>
#include <stdio.h>
#include <unistd.h>
class Socket
:public NonCopyable{
public:
    Socket();
    explicit Socket(int fd);
    ~Socket();
    int getSocket();
    int fd() const {return _fd;}
    void shutdownwrite();
private:
    int _fd;
};
#endif