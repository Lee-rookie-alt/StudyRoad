#include "TcpConnection.hpp"
#include <sstream>
#include <iostream>
using std::ostream;
using std::cout;
using std::endl;
TcpConnection::TcpConnection(int fd,EventLoop* loop)
:_sock(fd)
,_sockio(fd)
,_localaddr(getLocalAddress())
,_peeraddr(getPeerAddress())
,_loop(loop)
{}
TcpConnection::~TcpConnection(){}
InetAddress TcpConnection::getLocalAddress(){
    struct sockaddr_in local;
    memset(&local,0,sizeof(local));
    socklen_t len=sizeof(local);
    int ret=getsockname(_sock.fd(),(struct sockaddr *)&local,&len);
    ERROR_CHECK(ret,-1,"getsockname local");
    return local;
}
InetAddress TcpConnection::getPeerAddress(){
    struct sockaddr_in peer;
    memset(&peer,0,sizeof(peer));
    socklen_t len=sizeof(peer);
    int ret=getpeername(_sock.fd(),(struct sockaddr *)&peer,&len);
    ERROR_CHECK(ret,-1,"getsockname peer");
    return peer;
}
void TcpConnection::send(const string &msg){
    _sockio.writen(msg.c_str(),strlen(msg.c_str()));
}

string TcpConnection::receive(){
    char buf[65535]={0};
    int ret=_sockio.readline(buf,sizeof(buf));
    ERROR_CHECK(ret,-1,"readline");
    return string(buf,ret);
}

void TcpConnection::shutdown(){
    _sock.shutdownwrite();
}
string TcpConnection::to_string(){
    std::ostringstream oss;
    oss << "> tcp socket " << _sock.fd() 
        << ", " << _localaddr.ip() << ":" << _localaddr.port()
        << "->"      << _peeraddr.ip() << ":" << _peeraddr.port();
    return oss.str();
}
void TcpConnection::setNewConnectionCallback(const TcpConnectionCallback & cb){//不能用右值引用
    _onNewConnection=cb;
}
void TcpConnection::setMessageCallback(const TcpConnectionCallback &cb){
    _onMessage=cb;
}
void TcpConnection::setCloseCallback(const TcpConnectionCallback &cb){
    _onClose=cb;
}
void TcpConnection::handleNewConnectionCallback(){
    if(_onNewConnection){
        _onNewConnection(shared_from_this());
    }else{
        cout<<"_onNewConnection == nullptr \n"<<endl;
    }
}
void TcpConnection::handleMessageCallback(){
    if(_onMessage){
        _onMessage(shared_from_this());
    }else{
        cout<<"_onMessage == nullptr \n"<<endl;
    }
}
void TcpConnection::handleCloseCallback(){
    if(_onClose){
        _onClose(shared_from_this());
    }else{
        cout<<"_onClose == nullptr \n"<<endl;
    }
}
bool TcpConnection::isClosed() const{
    char buf[20]={0};
    return _sockio.recvPeek(buf,sizeof(buf)) == 0;
}
void TcpConnection::sendInLoop(const string &msg){
    if(_loop){
        _loop->runInLoop(std::bind(&TcpConnection::send,this,msg));
    }
}