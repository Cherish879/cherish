#include "TcpReady.h"

inline void ErrorCheck(const int& ret,const string& msg)
{
    if(ret==-1){perror(msg.c_str());return ;}
}

TcpReady::TcpReady(const string& ip,unsigned short port):_sock(),_addr(ip,port){}

TcpReady::~TcpReady(){}

int TcpReady::accept()
{
    int netfd=::accept(_sock.fd(),nullptr,nullptr);
    ErrorCheck(netfd,"TcpReady::accept遇到问题");
    return netfd;
}

void TcpReady::init()
{
    reuse();
    bind();
    listen();
}

int TcpReady::getfd() const{return _sock.fd();}

void TcpReady::reuse()
{
    int num1=1;
    int num2=1;
    int ret=setsockopt(_sock.fd(),SOL_SOCKET,SO_REUSEADDR,&num1,sizeof(num1));
    ErrorCheck(ret,"TcpReady::reuse地址重用设置失败");
    ret=setsockopt(_sock.fd(),SOL_SOCKET,SO_REUSEPORT,&num2,sizeof(num2));
    ErrorCheck(ret,"TcpReady::reuse端口重用设置失败");
}

void TcpReady::bind()
{
    int ret=::bind(_sock.fd(),(struct sockaddr*)&_addr.getSockaddr(),sizeof(_addr));
    ErrorCheck(ret,"TcpReady::bind遇到问题");
}

void TcpReady::listen()
{
    int ret=::listen(_sock.fd(),128);
    ErrorCheck(ret,"TcpReady::listen遇到问题");
}

