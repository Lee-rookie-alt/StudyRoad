#ifndef __INETADDRESS_HPP
#define __INETADDRESS_HPP
#include <netHeader.h>
#include <string>
#include <string.h>
using std::string;
class InetAddress
{
public:
    InetAddress(unsigned short port,const string & ip="0.0.0.0");
    InetAddress(const struct sockaddr_in &);
    ~InetAddress();
    string ip();
    unsigned short port();
    const struct sockaddr_in* get_InetAddressptr();
private:
    struct sockaddr_in _addr;
};
#endif