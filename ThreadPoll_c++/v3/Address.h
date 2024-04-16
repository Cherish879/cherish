#ifndef  __ADDRESS_H__
#define  __ADDRESS_H__

#include <string>
#include <arpa/inet.h>//struct sockaddr_in

using std::string;

class Address
{
public:
    Address(const string&,unsigned short&);         //初始化数据成员
    Address(const struct sockaddr_in&);             
    ~Address();                                     //啥也不干
    string getIP();                                 //返回ip地址的点分十进制
    unsigned short getPort();                       //返回端口号
    const struct sockaddr_in& getSockaddr()const;   //返回数据成员_addr
private:
    struct sockaddr_in _addr;
};

#endif
