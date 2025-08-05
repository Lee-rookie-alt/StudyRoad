#include "EchoServer.hpp"
#include <iostream>
using std::cout;
using std::endl;
EchoServer::EchoServer(size_t threadNum,size_t quesize,const string & ip,unsigned short port)
:_pool(threadNum,quesize)
,_server(port,ip)
{

}
EchoServer::~EchoServer(){}
void EchoServer::start(){
    _pool.start();
    using namespace std::placeholders;
    _server.setAllCallback(std::bind(&EchoServer::onNewConnection,this,_1)
                            ,std::bind(&EchoServer::onMessage,this,_1)    
                            ,std::bind(&EchoServer::onClose,this,_1));
    _server.start();
}
void EchoServer::stop(){
    _pool.stop();
    _server.stop();
}
void EchoServer::onNewConnection(const TcpConnectionPtr &conn){
    cout<<conn->to_string()<<" has connected!"<<endl;
}
void EchoServer::onMessage(const TcpConnectionPtr &r){

}
void EchoServer::onClose(const TcpConnectionPtr &r){

}
/*void EchoServer::setAllcallBack(const TcpConnectionPtr &NewConnection,const TcpConnectionPtr &Message,const TcpConnectionPtr &Close){
    
}*/