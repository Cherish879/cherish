#include <iostream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <strings.h>
#include <arpa/inet.h>
#include <fstream>
#include <sstream>
#include <cstring>
#include <fcntl.h>

using std::string;
using std::cout;
using std::endl;
using std::cerr;

//ERROR_CHECK封为内联函数
inline void ErrorCheck(int ret,const string& msg)
{
    if(ret==-1)
    {
        perror(msg.c_str());
        exit(-1);
    }
}

//服务端
class HttpServer
{
public:
    HttpServer(const string& ip,unsigned short port);
    ~HttpServer();
    void loop();
private:
    void serverInit();
    void sendIMG();
private:
    string _ip;
    unsigned short _port;
    int _sockfd;
    int _netfd;
};

//初始化各个数据成员
HttpServer::HttpServer(const string& ip,unsigned short port)
    :_ip(ip),_port(port),_sockfd(-1),_netfd(-1)
{
    serverInit();
}

//关闭sockfd
HttpServer::~HttpServer()
{
    if(_sockfd>0){close(_sockfd);}
}

//服务端主程序，事件循环
void HttpServer::loop()
{
    while(1)
    {
        _netfd=accept(_sockfd,nullptr,nullptr);
        char buf[BUFSIZ]={0};
        ssize_t ret=recv(_netfd,buf,sizeof(buf),0);
        cout<<"收到客户端消息，数据长度为"<<ret<<"  数据如下："<<endl;
        cout<<buf<<endl;
        sendIMG();
        close(_netfd);
        _netfd=-1;
    }
}

//socket、bind、listen什么的
void HttpServer::serverInit()
{
    //1、sockfd建立
    _sockfd=socket(AF_INET,SOCK_STREAM,0);
    ErrorCheck(_sockfd,"socket建立失败");

    //2、设置端口和地址重用
    int temp1=1,temp2=1;
    int ret=setsockopt(_sockfd,SOL_SOCKET,SO_REUSEADDR,&temp1,sizeof(temp1));
    ErrorCheck(ret,"设置地址重用失败");
    ret=setsockopt(_sockfd,SOL_SOCKET,SO_REUSEPORT,&temp2,sizeof(temp2));
    ErrorCheck(ret,"设置端口重用失败");
    
    //3、bind
    struct sockaddr_in addr;
    bzero(&addr,sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_port=htons(_port);
    addr.sin_addr.s_addr=inet_addr(_ip.c_str());
    ret=::bind(_sockfd,(struct sockaddr*)&addr,sizeof(addr));
    ErrorCheck(ret,"bind失败");

    //4、listen
    ret=listen(_sockfd,20);
    ErrorCheck(ret,"listen失败");
}

//发送图片的成员函数
void HttpServer::sendIMG()
{
    //打开文件
    std::ifstream file("1.png",std::ios::binary);
    if (!file) {cerr<<"文件打开失败"<<endl;exit(-1);}

    //获取文件大小
    file.seekg(0, std::ios::end);
    size_t len = file.tellg();
    file.seekg(0, std::ios::beg);

    //构造HTTP头部
    std::ostringstream os;
    os<<"HTTP/1.1 200 OK\r\n"
      <<"Content-Type: image/png\r\n"
      <<"Content-Length:"<<len<<"\r\n"
      <<"Connection:close\r\n"
      <<"\r\n";

    //发送HTTP响应报文头部
    string result=os.str();
    send(_netfd,result.c_str(),result.size(),0);

    //发送正文
    char buf[BUFSIZ]={0};
    while (!file.eof())
    {
        file.read(buf, BUFSIZ);
        size_t temp=file.gcount();
        if(send(_netfd,buf,temp,0)<0)
        {
            perror("发送失败");
            break;
        }
    }
}

int main(void)
{
    HttpServer server("192.168.79.128",1234);
    server.loop();
    return 0;
}

