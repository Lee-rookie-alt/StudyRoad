#include "Acceptor.hpp"
#define MAX_LISTEN 20000
Acceptor::Acceptor(unsigned short port,const string & ip)
:_addr(port,ip)
,_sock()
{
}
Acceptor::~Acceptor(){}
void Acceptor::setReuseAddr(bool on){
    int one=on ? 1 : 0;
    int ret=setsockopt(_sock.fd(),SOL_SOCKET,SO_REUSEADDR,&one,sizeof(one));
    ERROR_CHECK(ret,-1,"setsockopt addr");
}
void Acceptor::setReusePort(bool on){
    int one=on?1:0;
    int ret=setsockopt(_sock.fd(),SOL_SOCKET,SO_REUSEPORT,&one,sizeof(one));
    ERROR_CHECK(ret,-1,"setsockpt port");
}
void Acceptor::ready(){
    setReuseAddr(true);
    setReusePort(true);
    bind();
    listen();
}
void Acceptor::bind(){
    int ret=::bind(_sock.fd(),(const struct sockaddr *)&_addr,sizeof(_addr));
    ERROR_CHECK(ret,-1,"bind");
}
void Acceptor::listen(){
    int ret=::listen(_sock.fd(),MAX_LISTEN);
    ERROR_CHECK(ret,-1,"listen");
}
int Acceptor::accept(){
    int ret=::accept(_sock.fd(),nullptr,nullptr);
    ERROR_CHECK(ret,-1,"accept");
    return ret;
}