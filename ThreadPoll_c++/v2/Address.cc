#include <strings.h>
#include "Address.h"

Address::Address(const string& ip,unsigned short& port)
{
    bzero(&_addr,sizeof(struct sockaddr_in));
    _addr.sin_family=AF_INET;
    _addr.sin_port=htons(port);
    _addr.sin_addr.s_addr=inet_addr(ip.c_str());
}

Address::Address(const struct sockaddr_in& addr)
    :_addr(addr){}

Address::~Address(){}              

//将网络字节序的IP地址转换成点分十进制的字符串形式。
string Address::getIP(){return string(inet_ntoa(_addr.sin_addr));}          

unsigned short Address::getPort(){return ntohs(_addr.sin_port);}                                  

const struct sockaddr_in& Address::getSockaddr()const{return _addr;}

