#ifndef  __CLIENTMGG_H__
#define  __CLIENTMGG_H__

#include <memory>
#include <functional>
#include "IO.h"
#include "Socket.h"
#include "Address.h"

using std::shared_ptr;
using std::function;
class ConnectMSG;
using f=function<void(const shared_ptr<ConnectMSG>&)>;

class ConnectMSG
:public std::enable_shared_from_this<ConnectMSG>
{
public:
    ConnectMSG(int);    //接个fd，给_io和_sock初始化用
    ~ConnectMSG();      //啥也不干
    void send(const string&);   //调用_io的write函数
    string recv();          //调用_io的readline函数
    bool life()const;       //目标fd中有数据可读取就返回1，否则0

    string status();        //打印服务端和对应客户端消息的函数
    
    void setNewFunc(const f& func);     //设置新连接回调函数
    void setMSGFunc(const f& func);     //设置收到消息回调函数
    void setCloseFunc(const f& func);   //设置关闭连接回调函数

    void NewFunc();         //新连接回调函数
    void MSGFunc();         //收到消息回调函数
    void CloseFunc();       //关闭连接回调函数
private:
    Address ClientInit();   //初始化_Client
    Address ServerInit();   //初始化_Server
private:
    IO _io;
    Socket _sock;
    Address _Client;    //存储对应客户端sockaddr_in信息
    Address _Server;    //存储服务器本端sockaddr_in信息

    f _newfunc;
    f _msgfunc;
    f _closefunc;
};

#endif
