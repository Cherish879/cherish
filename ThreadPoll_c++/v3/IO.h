#ifndef __IO_H__
#define __IO_H__

#include <string>
using std::string;

class IO
{
public:
    IO(int);                    //仅接收fd来构造自身
    ~IO();                      //close
    string readAll(int);        //全读取
    string readLine();          //读一行
    void write(const string&);  //把传入字符串写入fd
private:
    int _fd;
};

#endif
