#ifndef __ACCEPTOR_HPP
#define __ACCEPTOR_HPP
#include "InetAddress.hpp"
#include "Socket.hpp"
#include <netHeader.h>
class Acceptor{
public:
    Acceptor(unsigned short,const string & ip ="0.0.0.0");
    ~Acceptor();
    void ready();
    void setReuseAddr(bool);
    void setReusePort(bool);
    void bind();
    void listen();
    int accept();
    unsigned short getPort(){return _addr.port();}
    string getIP(){return _addr.ip();}
    int getfd(){return _sock.fd();}
private:
    InetAddress _addr;
    Socket      _sock;
};
#endif