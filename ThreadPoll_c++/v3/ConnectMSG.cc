#include <iostream>
#include <string>
#include "ConnectMSG.h"

using std::cout;
using std::endl;


inline void ErrorCheck(int ret,const string& msg)
{
    if(ret==-1){perror("msg");return;}
}

ConnectMSG::ConnectMSG(int fd)
:_io(fd),_sock(fd),_Client(ClientInit()),_Server(ServerInit()){}

ConnectMSG::~ConnectMSG(){}

void ConnectMSG::send(const string& msg)
{
    _io.write(msg);
}

string ConnectMSG::recv()
{
    return _io.readLine();
}

bool ConnectMSG::life()const
{
    char temp[10]={0};
    int ret=::recv(_sock.fd(),temp,10,MSG_PEEK);
    return ret!=0;
}

string ConnectMSG::status()
{
    string result;
    result+="服务端";
    result+=_Server.getIP();
    result+=":";
    result+=std::to_string(_Server.getPort());
    result+="------>";
    result+="客户端";
    result+=_Client.getIP();
    result+=":";
    result+=std::to_string(_Client.getPort());
    return result;
}

void ConnectMSG::setNewFunc(const f& func)
{
    _newfunc=func;    
}

void ConnectMSG::setMSGFunc(const f& func)
{
    _msgfunc=func;
}

void ConnectMSG::setCloseFunc(const f& func)
{
    _closefunc=func;
}

void ConnectMSG::NewFunc()
{
    if(_newfunc){_newfunc(shared_from_this());}
    else{cout<<"新连接的回调函数为空"<<endl;}
}

void ConnectMSG::MSGFunc()
{
    if(_msgfunc){_msgfunc(shared_from_this());}
    else{cout<<"处理客户端消息的回调函数为空"<<endl;}
}

void ConnectMSG::CloseFunc()
{
    if(_closefunc){_closefunc(shared_from_this());}
    else{cout<<"关闭客户端的回调函数为空"<<endl;}
}

Address ConnectMSG::ClientInit()
{
    struct sockaddr_in addr;
    socklen_t len= sizeof(struct sockaddr);
    int ret=getpeername(_sock.fd(),(struct sockaddr*)&addr,&len);
    ErrorCheck(ret,"ClientInit失败");
    return Address(addr);
}

Address ConnectMSG::ServerInit()
{
    struct sockaddr_in addr;
    socklen_t len= sizeof(struct sockaddr);
    int ret=getsockname(_sock.fd(),(struct sockaddr*)&addr,&len);
    ErrorCheck(ret,"ClientInit失败");
    return Address(addr);
}

