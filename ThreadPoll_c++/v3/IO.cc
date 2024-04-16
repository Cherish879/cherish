#include <sys/socket.h>
#include <cstring>
#include <unistd.h>
#include "IO.h"

IO::IO(int fd):_fd(fd){}

IO::~IO(){close(_fd);}

string IO::readAll(int len)
{
    string result;
    int ret;
    while(len!=0)
    {
        char cc[BUFSIZ];
        memset(cc,0,BUFSIZ);
        ret=recv(_fd,cc,len>BUFSIZ?BUFSIZ:len,MSG_WAITALL);
        if(ret>0)
        {
            len-=ret;
            result+=cc;
        }
        else if(ret==-1 && errno==EINTR){continue;}
        else if(ret==-1){perror("readAll遇到问题");return "";}
    }
    return result;
}

string IO::readLine()
{
    string result;
    int ret;
    while(1)
    {
        char cc[BUFSIZ];
        memset(cc,0,BUFSIZ);
        ret=recv(_fd,cc,BUFSIZ,MSG_PEEK);
        if(ret>0)             
        {
            for(int i=0;i<ret;++i)
            {
                if(cc[i]=='\n')
                {
                    memset(cc,0,BUFSIZ);
                    recv(_fd,cc,i,0);
                    result+=cc;
                    memset(cc,0,BUFSIZ);
                    recv(_fd,cc,1,0);//把剩下的\n读走
                    return result;//不遇到错误的情况下，这段while(1)循环以及整个函数的唯一出口
                }
            }
            memset(cc,0,BUFSIZ);
            recv(_fd,cc,BUFSIZ,0);
            result+=cc;       
        }                     
        else if(ret==-1){perror("readLine遇到问题");return "";}
    }
}

void IO::write(const string& msg)
{
    size_t len=msg.length();
    size_t count=0;
    const char* temp=msg.c_str();
    while(count<len)
    {
        int ret=send(_fd,temp+count,len-count,0);
        if(ret==-1)
        {
            if(errno == EINTR){continue;}
            perror("IO::write遇到错误");
            break;
        }
        count+=ret;
    }
}

