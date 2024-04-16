#include <unistd.h>
#include <string>
#include "Event.h"
#include "TcpReady.h"
#include "ConnectMSG.h"

using std::cout;
using std::endl;
using std::string;

inline void ErrorCheck(int ret,const string& msg)
{
    if(ret==-1){perror(msg.c_str());return;}
}

Event::Event(TcpReady& tcpready)
:_epfd(EpollInit()),_clientnum(0),_readylist(1024)
,_life(false),_tcpready(tcpready)
{
    EpollAdd(_tcpready.getfd());
}
 
Event::~Event(){close(_epfd);}

void Event::loop()
{
    _life=true;
    while(_life){EpollWait();}
}

void Event::unloop(){_life=false;}

void Event::setNewFunc(const f&& func){_newfunc=func;}
void Event::setMSGFunc(const f&& func){_msgfunc=func;}
void Event::setCloseFunc(const f&& func){_closefunc=func;}

int Event::EpollInit()
{
    int fd=epoll_create(1);
    ErrorCheck(fd,"EpollInit失败");
    return fd;
}

void Event::EpollAdd(int fd)
{
    struct epoll_event temp;
    temp.events=EPOLLIN;
    temp.data.fd=fd;
    int ret=epoll_ctl(_epfd,EPOLL_CTL_ADD,fd,&temp);
    ErrorCheck(ret,"EpollAdd失败");
}

void Event::EpollDel(int fd)
{
    struct epoll_event temp;
    temp.events=EPOLLIN;
    temp.data.fd=fd;
    int ret=epoll_ctl(_epfd,EPOLL_CTL_DEL,fd,&temp);
    ErrorCheck(ret,"EpollDel失败");
}

void Event::EpollWait()
{
    int readynum=0;
    do
    {
        readynum=epoll_wait(_epfd,&_readylist[0],_readylist.size(),3000);
    }while(readynum==-1 && errno==EINTR);
    
    if(readynum==0){cout<<"epoll_wait计时归零"<<endl;}
    else if(readynum==-1){cout<<"epoll_wait遇到错误"<<endl;return;}
    else
    {
        if(_clientnum==_readylist.size()){_readylist.reserve(2*_readylist.capacity());}
        for(int i=0;i<readynum;++i)
        {
            if(_readylist[i].data.fd==_tcpready.getfd())
            {
                if(_readylist[i].events&EPOLLIN){NewFunc();++_clientnum;}
            }
            else
            {
                if(_readylist[i].events&EPOLLIN){MSGFunc(_readylist[i].data.fd);}
            }
        }

    }
}

void Event::NewFunc()
{
    int netfd=_tcpready.accept();
    ErrorCheck(netfd,"NewFunc失败");
    
    EpollAdd(netfd);
    auto temp=std::make_shared<ConnectMSG>(netfd);

    temp->setNewFunc(_newfunc);
    temp->setMSGFunc(_msgfunc);
    temp->setCloseFunc(_closefunc);

    _conns[netfd]=temp;

    temp->NewFunc();
}

void Event::MSGFunc(int fd)
{
    auto it=_conns.find(fd);
    if(it!=_conns.end())
    {
        if(!it->second->life())
        {
            it->second->CloseFunc();
            EpollDel(fd);
            _conns.erase(it);
        }
        else{it->second->MSGFunc();}
    }
    else{cout<<"连接不存在！"<<endl;}
}

