#include "InetAddress.hpp"
InetAddress::InetAddress(unsigned short port,const string & ip){
    memset(&_addr,0,sizeof(_addr));
    _addr.sin_family=AF_INET;
    _addr.sin_addr.s_addr=inet_addr(ip.c_str());
    _addr.sin_port=htons(port);
}
InetAddress::InetAddress(const struct sockaddr_in & addr)
:_addr(addr)
{}
InetAddress::~InetAddress(){}
string InetAddress::ip(){
    return string(inet_ntoa(_addr.sin_addr));
}
unsigned short InetAddress::port(){
    return ntohs(_addr.sin_port);
}
const struct sockaddr_in * InetAddress::get_InetAddressptr(){
    return &_addr;
}