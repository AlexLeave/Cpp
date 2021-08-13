/**
 * @file socket.hpp
 * @author csu-lf (csu.lifeng@qq.com)
 * 
 * socket改进版，对于编程逻辑更加清晰
 * 
 * @version 2.0
 * @date 2021-08-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __MY_SOCKET_H__
#define __MY_SOCKET_H__ 1


#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h> 
#include <string>
#include <cstring>
#include <stdarg.h>


#define DEBUGMODE 1

// 父类socket
#define SOCKETERR -1
#define BUFERR -2
class MySocket
{
    protected:
    // socket文件描述符
    int _socket = -1;
    struct sockaddr_in LocalSocketAddr;
    // 缓冲区
    void* _buf = NULL;
    int BUFSIZE = 128;
    // 保存上一次通讯的远端socket信息
    struct sockaddr_in _remoteAddr;
    // errno
    int _errno = 0;

    int init(std::string type);
    int init(int socket);


    int BufSize();
    int SetBufSize(int size);
    void ClearBuf() { memset(_buf, 0, BUFSIZE); }
    char* LocalBindIP() { return inet_ntoa(LocalSocketAddr.sin_addr); }
    int LocalBindPort() { return ntohs(LocalSocketAddr.sin_port); }
   
    public:
    /**
     * @brief Construct a new CSocket object
     * 
     * @param type 填“udp”创建udp socket，否则为tcp socket
     */
    MySocket(std::string type = "tcp")
    {
        _errno = init(type);
        if (_errno < 0)
        {
            printf("error,you can use function Err() to see what happened\n");            
        }
    }

    /**
     * @brief 把一个 socket 交给 CSocket类 来管理
     * 
     * @param socket 交给我的socket
     */
    MySocket(int socket)
    {
        _errno = init(socket);
        if (_errno < 0)
        {
            printf("error,you can use function Err() to see what happened\n");
        }
    }

    ~MySocket()
    {
        if (_buf)
        {
            free(_buf);
        }
    }

    int Socket();
    void* Buf();

    int Err();

    int Bind(int port);
    int Bind(const char* ip, int port);

    int Close() { return close(_socket); }
};



class Udp : public MySocket
{
    protected:

    public:
    Udp():MySocket("udp")
    {
        
    }

    Udp(int socket):MySocket(socket)
    {

    }

};
class Tcp : public MySocket
{
    protected:

    public:
    Tcp():MySocket("tcp")
    {
        
    }
    Tcp(int socket):MySocket(socket)
    {

    }
    // 用于服务端生成的连接客户端socket保存
    Tcp(int socket, struct sockaddr_in* remoteAddr):MySocket(socket)
    {
        memcpy(&_remoteAddr, remoteAddr, sizeof(struct sockaddr_in));
    }

    int Recv();
    int Send(void* ptr, int size);
    bool IsConnect();
};




class UdpServer : public Udp
{
    protected:
    struct sockaddr_in *clntAddr = &_remoteAddr;

    public:
    UdpServer():Udp()
    {

    }

    UdpServer(int socket):Udp(socket)
    {

    }

    int Bind(int port);
    int Bind(const char* ip, int port);

    int Recv();
    int Send(void* ptr, int size);

    char* ClientIP();
    int ClientPort();

};
class UdpClient : public Udp
{
    protected:
    struct sockaddr_in *serverAddr = &_remoteAddr;

    public:
    int Connect(const char * ip, int port);
    int Send(void* ptr, int size);
    int Recv();
};




class TcpServer : public Tcp
{
    protected:
    struct sockaddr_in* clntAddr = &_remoteAddr;

    public:
    TcpServer():Tcp()
    {

    }
    TcpServer(int socket):Tcp(socket)
    {

    }
    TcpServer(char* ip, int port):Tcp()
    {
        Bind(ip, port);
    }
    


    int Listen(int n);
    int Accept();

    int IsConnect();

    struct sockaddr_in* ClientAddr();
    char* ClientIP();
    int ClientPort();

};
class TcpClient : public Tcp
{
    protected:
    struct sockaddr_in *serverAddr = &_remoteAddr;

    public:
    TcpClient():Tcp()
    {

    }
    TcpClient(int socket):Tcp(socket)
    {

    }
    

    int Connect(const char* ip, int port);
};








int MySocket::init(std::string type = "tcp")
{
    memset(&LocalSocketAddr, 0, sizeof(struct sockaddr_in));
    memset(&_remoteAddr, 0, sizeof(struct sockaddr_in));
    if (type == "udp")
    {
        _socket = socket(AF_INET, SOCK_DGRAM, 0);
    }
    else
    {
        _socket = socket(AF_INET, SOCK_STREAM, 0);
    }

    if (_socket < 0)
    {
        #if DEBUGMODE
        printf("error occurred at %s.%d", __FILE__, __LINE__);
        #endif
        return SOCKETERR;
    }

    _buf = malloc(BUFSIZE);

    if (!_buf)
    {
        #if DEBUGMODE
        printf("error occurred at %s.%d", __FILE__, __LINE__);
        #endif
        return BUFERR;
    }
    ClearBuf();

    return 0;

}


int MySocket::init(int socket)
{
    if (socket > 1)
            _socket = socket;
    else
    {
        #if DEBUGMODE
        printf("error occurred at %s.%d", __FILE__, __LINE__);
        #endif
        return SOCKETERR;
    }
            
    _buf = malloc(BUFSIZE);

    if (!_buf)
    {
        #if DEBUGMODE
        printf("error occurred at %s.%d", __FILE__, __LINE__);
        #endif
        return BUFERR;
    }
    ClearBuf();

    return 0;
}


/**
 * @brief 打印错误提示
 * 
 * @return int _errno
 */
int MySocket::Err()
{
    if (_errno == SOCKETERR)
    {
        printf("create socket failed\n");
    }
    else if (_errno == BUFERR)
    {
        printf("create buf failed\n");
    }
    else
    {
        printf("error undefined, see the return number to guess what happened\n");
    }

    return _errno;
}


/**
 * @brief 返回 socket 文件描述符
 * 
 * @return int 
 */
int MySocket::Socket()
{
    return _socket;
}

/**
 * @brief 返回接收缓冲区
 * 
 * @return void* buf缓冲区
 */
void * MySocket::Buf()
{
    return _buf;
}

/**
 * @brief 返回缓冲区大小
 * 
 * @return int 
 */
int MySocket::BufSize()
{
    return BUFSIZE;
}


/**
 * @brief 设置buf大小
 * 
 * @param size 
 * @return int 成功 0，失败 < 0。失败buf不变
 */
int MySocket::SetBufSize(int size)
{
    void *tmpbuf;
    if (_buf)
    {
        tmpbuf = malloc(size);
        if (!tmpbuf)
        {
            _errno = BUFERR;
            return _errno;
        }
        free(_buf);
        _buf = tmpbuf;
    }

    BUFSIZE = size;
    return 0;
}


/**
 * @brief Socket绑定本机端口，默认监听所有网卡（0.0.0.0）
 * 
 * @param port 端口号
 * @return int 成功 0，失败-1
 */
int MySocket::Bind(int port)
{
    memset(&LocalSocketAddr, 0, sizeof(LocalSocketAddr));
    LocalSocketAddr.sin_family = AF_INET;
    LocalSocketAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    LocalSocketAddr.sin_port = htons(port);

    return bind(_socket, (struct sockaddr *)&LocalSocketAddr, sizeof(LocalSocketAddr));        
}



/**
 * @brief Socket绑定本机端口，默认监听所有网卡（0.0.0.0）
 * 
 * @param port 端口号
 * @return int 成功 0，失败-1
 */
int MySocket::Bind(const char *ip, int port)
{
    memset(&LocalSocketAddr, 0, sizeof(LocalSocketAddr));
    LocalSocketAddr.sin_family = AF_INET;
    // LocalSocketAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if ((LocalSocketAddr.sin_addr.s_addr = inet_addr(ip)) == -1 )
        return -1;
    LocalSocketAddr.sin_port = htons(port);

    return bind(_socket, (struct sockaddr *)&LocalSocketAddr, sizeof(LocalSocketAddr));        
}








/**
 * @brief Socket绑定本机端口，默认监听所有网卡（0.0.0.0）
 * 
 * @param port 端口号
 * @return int 成功 0，失败-1
 */
int UdpServer::Bind(int port)
{
    memset(&LocalSocketAddr, 0, sizeof(LocalSocketAddr));
    LocalSocketAddr.sin_family = AF_INET;
    LocalSocketAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    LocalSocketAddr.sin_port = htons(port);

    return 0;  
}
/**
 * @brief Socket绑定本机端口，默认监听所有网卡（0.0.0.0）
 * 
 * @param port 端口号
 * @return int 成功 0，失败-1
 */
int UdpServer::Bind(const char *ip, int port)
{
    memset(&LocalSocketAddr, 0, sizeof(LocalSocketAddr));
    LocalSocketAddr.sin_family = AF_INET;
    if ((LocalSocketAddr.sin_addr.s_addr = inet_addr(ip)) == -1 )
        return -1;
    LocalSocketAddr.sin_port = htons(port);

    return 0;       
}


/**
 * @brief 接收消息
 * 
 * @return int 成功接收的字节数 >= 0，失败 -1
 */
int UdpServer::Recv()
{
    socklen_t clntAddrLen = sizeof(struct sockaddr_in);
    ClearBuf();
    return recvfrom(Socket(), Buf(), BufSize(), 0, (struct sockaddr*)clntAddr, &clntAddrLen);
}


/**
 * @brief 发送消息，在此之前请先接收消息获取到客户端socket，否则请使用sendto函数自行指定
 * 
 * @param ptr 发送消息保存的位置
 * @param size 消息大小
 * @return int 成功发送的字节数 >= 0，失败 -1
 */
int UdpServer::Send(void* ptr, int size)
{
    socklen_t clntAddrLen = sizeof(struct sockaddr_in);
    return sendto(Socket(), ptr, size, 0, (struct sockaddr*)clntAddr, clntAddrLen);
}


/**
 * @brief 返回获取到客户端的IP
 * 
 * @return char* 
 */
char* UdpServer::ClientIP()
{
    return inet_ntoa(clntAddr->sin_addr);
}

/**
 * @brief 返回获取到客户端的端口
 * 
 * @return int 
 */
int UdpServer::ClientPort()
{
    return ntohs(clntAddr->sin_port);
}






/**
 * @brief 设置服务端，并非连接。假连接
 * 
 * @param ip 服务器地址
 * @param port 服务器开放连接的端口号
 * @return int 恒为0
 */
int UdpClient::Connect(const char * ip, int port)
{
    memset(serverAddr, 0, sizeof(struct sockaddr_in));
    serverAddr->sin_family = AF_INET;
    serverAddr->sin_addr.s_addr = inet_addr(ip);
    serverAddr->sin_port = htons(port);

    // return connect(Socket(), (struct sockaddr *)serverAddr, sizeof(struct sockaddr_in));
    return 0;
}



/**
 * @brief 向服务端发送消息
 * 
 * @param ptr 消息保存地址
 * @param size 消息大小
 * @return int 成功 >= 0，失败 -1
 */
int UdpClient::Send(void* ptr, int size)
{
    socklen_t serverAddrLen = sizeof(struct sockaddr_in);
    return sendto(Socket(), ptr, size, 0, (sockaddr*)serverAddr, serverAddrLen);
}


/**
 * @brief 返回接收字符多少个
 * 
 * @return int 成功 >= 0，失败 -1
 */
int UdpClient::Recv()
{
    ClearBuf();
    return recvfrom(Socket(), Buf(), BufSize(), 0, NULL, NULL);
}

/**
 * tcp发送数据
 * 
 * @param ptr 要发送的数据
 * @param size 发送数据大小
 * @return int 成功 发送了多少数据 >=0，失败 -1
 */
int Tcp::Send(void* ptr, int size)
{
    return send(Socket(), ptr, size, 0);
}

/**
 * tcp接收数据保存到buf
 * 
 * @return int 成功 接收了多少字节数据 >= 0，失败 -1
 */
int Tcp::Recv()
{
    ClearBuf();
    return recv(Socket(), Buf(), BufSize(), 0);
}


/**
 * @brief 查看 socket 是否还连接着
 * 
 * @return true 未断开
 * @return false 已断开
 */
bool Tcp::IsConnect()
{
    struct tcp_info info; 
    int len = sizeof(info); 
    getsockopt(Socket(), IPPROTO_TCP, TCP_INFO, &info, (socklen_t *)&len);
    if(info.tcpi_state == TCP_ESTABLISHED) return true;
    else return false;
}




/**
 * @brief 设置监听模式和等待阻塞数量
 * 
 * @param n 
 * @return int 成功 0，失败 -1
 */
int TcpServer::Listen(int n = 20)
{
    return listen(Socket(), n);
}


/**
 * @brief 等待tcp客户端连接，返回连接到客户端的socket
 * 
 * @return int 连接到客户端的socket，请好好保存
 */
int TcpServer::Accept()
{
    socklen_t clntAddrLen = sizeof(struct sockaddr_in);
    return accept(_socket, (struct sockaddr*)&clntAddr, &clntAddrLen);
}


/**
 * @brief 返回连接到的客户端结构体指针
 * 
 * @return struct sockaddr_in* 
 */
struct sockaddr_in* TcpServer::ClientAddr()
{
    return clntAddr;
}



/**
 * @brief tcp客户端连接服务端
 * 
 * @param ip 服务器地址
 * @param port 服务器开放连接的端口号
 * @return int 成功 0，失败 -1
 */
int TcpClient::Connect(const char * ip, int port)
{
    memset(serverAddr, 0, sizeof(struct sockaddr_in));
    serverAddr->sin_family = AF_INET;
    serverAddr->sin_addr.s_addr = inet_addr(ip);
    serverAddr->sin_port = htons(port);

    return connect(Socket(), (struct sockaddr *)serverAddr, sizeof(struct sockaddr_in));
}




#endif