#ifndef  __SOCKET_H__
#define  __SOCKET_H__

#include "NoCopy.h"

class Socket:NoCopy
{
public:
    Socket();           //调用socket创个fd
    Socket(int fd);     //传入fd
    ~Socket();          //关掉_fd
    int fd() const;     //返回_fd
    void closeWrite();  //关闭_fd的写端
private:
    int _fd;
};

#endif
