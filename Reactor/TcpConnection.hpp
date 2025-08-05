#ifndef __TCPCONNECTION_HPP
#define __TCPCONNECTION_HPP
#include "InetAddress.hpp"
#include "Socket.hpp"
#include "SocketIO.hpp"
#include "InetAddress.hpp"
#include "EventLoop.hpp"
#include <functional>
#include <memory>
class EventLoop;
class TcpConnection:
public std::enable_shared_from_this<TcpConnection>
{
    using TcpConnectionPtr=std::shared_ptr<TcpConnection>;
    using TcpConnectionCallback=std::function<void (const TcpConnectionPtr)>;
    public:
    TcpConnection(int,EventLoop*);
    ~TcpConnection();
    void send(const string &);
    string receive();
    string to_string();
    void shutdown();
    void setNewConnectionCallback(const TcpConnectionCallback &cb);
    void setMessageCallback(const TcpConnectionCallback &cb);
    void setCloseCallback(const TcpConnectionCallback &cb);
    void handleNewConnectionCallback();
    void handleMessageCallback();
    void handleCloseCallback();
    bool isClosed() const;
    void sendInLoop(const string &msg);
private:    
    InetAddress getLocalAddress();
    InetAddress getPeerAddress();
private:
    SocketIO _sockio;
    Socket _sock;
    InetAddress _localaddr;
    InetAddress _peeraddr;
    TcpConnectionCallback _onNewConnection;
    TcpConnectionCallback _onClose;
    TcpConnectionCallback _onMessage;
    EventLoop *_loop;
};
#endif