#ifndef __ECHOSERVER_HPP
#define __ECHOSERVER_HPP
#include "ThreadPool.hpp"
#include "TcpServer.hpp"
class EchoServer{
public:
    EchoServer(size_t threadNum,size_t quesize,const string & ip,unsigned short port);
    ~EchoServer();
    void start();
    void stop();
    void onNewConnection(const TcpConnectionPtr &r);
    void onMessage(const TcpConnectionPtr &r);
    void onClose(const TcpConnectionPtr &r);
    void setAllcallBack();
private:
    ThreadPool _pool;
    TcpServer  _server;
};
#endif