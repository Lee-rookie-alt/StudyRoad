#include "Socket.hpp"
Socket::Socket(){
    _fd=socket(AF_INET,SOCK_STREAM,0);
    if(_fd<0){
        perror("socket");
    }
}
Socket::Socket(int fd)
:_fd(fd)
{}
void Socket::shutdownwrite(){
    shutdown(_fd,SHUT_WR);
}
Socket::~Socket(){close(_fd);}
int Socket::getSocket(){
    return _fd;
}