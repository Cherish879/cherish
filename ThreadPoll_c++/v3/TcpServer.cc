#include "TcpServer.h"
 
TcpServer::TcpServer(const string& ip,unsigned short port)
    :_tcpready(ip,port),_loop(_tcpready){}

TcpServer::~TcpServer(){}

void TcpServer::start()
{
    _tcpready.init();
    _loop.loop();
}

void TcpServer::stop()
{
    _loop.unloop();
}

void TcpServer::setAllCallBack(ff&& cb1,ff&& cb2,ff&& cb3)
{
    _loop.setNewFunc(std::move(cb1));
    _loop.setMSGFunc(std::move(cb2));
    _loop.setCloseFunc(std::move(cb3));
} 

