#ifndef __TCPREADY_H__
#define __TCPREADY_H__

#include "Socket.h"
#include "Address.h"

class TcpReady
{
public:
    TcpReady(const string&,unsigned short); //接ip和端口传给Adress构造用
    ~TcpReady();                            //啥也不干
    int accept();                           //同意连接并返回netfd
    void init();                            //端口重用，bind，listen三件套
    int getfd() const;                      //返回_sock成员的data.fd
private:
    void reuse();                           //地址、端口重用
    void bind();
    void listen();
private:
    Socket _sock;
    Address _addr;
};

#endif
