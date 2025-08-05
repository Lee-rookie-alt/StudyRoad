#ifndef __SOCKETIO_HPP
#define __SOCKETIO_CPP

#include <netHeader.h>
class SocketIO{
public:
    SocketIO(int fd);
    ~SocketIO();
    int readn(char * buf,int len);
    int writen(const char * buf,int len);
    int readline(char * buf,int len);
    int recvPeek(char *buf,int max) const;
private:
    int _fd;
};
#endif