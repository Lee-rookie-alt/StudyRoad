#ifndef __TCPSERVER_HPP
#define __TCPSERVER_HPP
#include "EventLoop.hpp"
class TcpServer{
public:
    TcpServer(unsigned short port,const string & ip);
    ~TcpServer();
    void start();
    void stop();
    void setAllCallback(TcpConnectionCallback &&connection,TcpConnectionCallback &&Message,TcpConnectionCallback &&Close);
private:
    Acceptor _accept;
    EventLoop _loop;
};
#endif