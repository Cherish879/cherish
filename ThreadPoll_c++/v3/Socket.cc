#include <sys/socket.h>
#include <string>
#include <unistd.h>
#include "Socket.h"

using std::string;

inline void ErrorCheck(int ret,const string& msg)
{
    if(ret==-1){perror(msg.c_str());return;}
}

Socket::Socket()
{
    _fd=::socket(AF_INET,SOCK_STREAM,0);
    ErrorCheck(_fd,"执行Socket::Socket失败");
}

Socket::Socket(int fd):_fd(fd){}

Socket::~Socket(){close(_fd);}

int Socket::fd()const{return _fd;}

void Socket::closeWrite()
{
    int ret=shutdown(_fd,SHUT_WR);
    ErrorCheck(ret,"执行Socket::closeWrite失败");
}
