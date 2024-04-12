#ifndef __EVENT_H__
#define __EVENT_H__

#include <memory>
#include <functional>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <sys/epoll.h>

using std::function;
using std::shared_ptr;
using std::vector;
using std::unordered_map;

class TcpReady;
class ConnectMSG;
using f=function<void(const shared_ptr<ConnectMSG>&)>;

class Event
{
public:
    Event(TcpReady&);   //传入的TcpReady给_tcpready初始化，vector容量1024
    ~Event();           //close(epfd)
    void loop();        //设置_life为true，根据_life无限调用EpollWait
    void unloop();      //设置_life为false
    void setNewFunc(const f&& func);
    void setMSGFunc(const f&& func);
    void setCloseFunc(const f&& func);    
private:
    int EpollInit();    //epoll_create
    void EpollAdd(int fd);
    void EpollDel(int fd);
    void EpollWait();
    void NewFunc();
    void MSGFunc(int fd);
private:
    int _epfd;      //创的epfd
    size_t _clientnum;  //仅记录连接过的数目而不删除
    vector<struct epoll_event> _readylist;  //用来存储就序集合
    bool _life;     //loop函数结束标志位
    TcpReady& _tcpready;    
    unordered_map<int,shared_ptr<ConnectMSG>> _conns;   //fd和ConnectMSG类的对应关系

    f _newfunc;
    f _msgfunc;
    f _closefunc;
};

#endif
