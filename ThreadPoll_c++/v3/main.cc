
#include <iostream>
#include <unistd.h>
#include "TcpReady.h"
#include "ConnectMSG.h"
#include "Event.h"
#include "TcpServer.h"

using std::cout;
using std::endl;
using std::move;

void newfunc(const std::shared_ptr<ConnectMSG>& temp)
{
    cout<<temp->status()<<"已连接至服务器"<<endl;
}

void msgfunc(const std::shared_ptr<ConnectMSG>& temp)
{
    string result="服务端回复：";
    string cc=temp->recv();
    cout<<temp->status()<<"收到消息:"<<cc<<endl;
    cc+='\n';
    result+=cc;
    temp->send(result);
}

void closefunc(const std::shared_ptr<ConnectMSG>& temp)
{
    cout<<temp->status()<<"已从服务器断开"<<endl;
}

void test()
{
    TcpServer server("127.0.0.1",1234);
    server.setAllCallBack(std::move(newfunc),
                          std::move(msgfunc),
                          std::move(closefunc));
    server.start();
}

int main(void)
{
    test();
    return 0;
}
