#include "TcpServer.hpp"
TcpServer::TcpServer(unsigned short port,const string & ip)
:_accept(port,ip)
,_loop(_accept)
{}
TcpServer::~TcpServer(){}
void TcpServer::start(){
    _accept.ready();
    _loop.Loop();
}
void TcpServer::stop(){
    _loop.unLoop();
}
void TcpServer::setAllCallback(TcpConnectionCallback &&connection,TcpConnectionCallback &&Message,TcpConnectionCallback &&Close){
    _loop.setNewConnectionCallback(std::move(connection));
    _loop.setMessageCallback(std::move(Message));
    _loop.setCloseCallback(std::move(Close));
}
