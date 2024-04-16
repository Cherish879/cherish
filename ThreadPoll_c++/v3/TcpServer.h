#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__

#include "TcpReady.h"
#include "Event.h"

using ff=std::function<void(const shared_ptr<ConnectMSG>&)>;

class TcpServer
{
public:
    TcpServer(const string&,unsigned short);
    ~TcpServer();
    void start();
    void stop();
    void setAllCallBack(ff&& cb1,ff&& cb2,ff&& cb3);
private:
    TcpReady _tcpready;
    Event _loop;
};

#endif
