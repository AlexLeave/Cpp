/**
 * @file socket.hpp
 * @author csu-lf (csu.lifeng@qq.com)
 * @brief 封装了常用的网络编程的socket成CSocket类，请愉快的使用吧
 * 0.2 版本更改了类部分结构，让它更加适合多线程编程
 * 1.1 版本udp使用更加友好，添加RecvFrom等实用函数
 * 
 * @version 1.1
 * @date 2021-07-02
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef _MY_SOCKET_
#define _MY_SOCKET_ 1

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



/**
 * @brief 使用C++类封装socket，创建socket只需要创建这个类，服务端socket使用server开头命名的函数
 * 客户端socket使用client开头命名的函数
 * 
 */
class CSocket{
    private:
    // socket文件描述符
    int _socket;
    // 测试接收web页面专用大小5k大小buf
    char buf[5*1024]; 
    // 结构体，存放客户端socket信息。对于udp服务端这是会变化的
    // 对于Accpect后存放的是tcp连接客户端。对于RecvFrom后存放的是会话远端
    struct sockaddr_in clntAddr; 
    // socket类型，若是托管socket且用户没有规定，默认视为tcp
    std::string _type = "tcp";


    public:
    /**
     * @brief Construct a new CSocket object
     * 
     * @param type 填“udp”创建udp socket，否则为tcp socket
     */
    CSocket(std::string type = "tcp")
    {
        _type = type;
        if (type == "udp")
        {
            _socket = socket(AF_INET, SOCK_DGRAM, 0);
        }
        else
        {
            _socket = socket(AF_INET, SOCK_STREAM, 0);
        }
        memset(buf, 0, sizeof(buf));
    }

    /**
     * @brief 把一个 socket 交给 CSocket类 来管理
     * 
     * @param socket 交给我的socket
     */
    CSocket(int socket)
    {
        if (socket > 1)
            _socket = socket;
        else 
            printf("err: socket fd < 2\n");
    }

    /**
     * @brief 把一个 socket 交给 CSocket类 来管理
     * 
     * @param socket 交给我的socket
     * @param type socket类型
     */
    CSocket(int socket, std::string type)
    {
        _socket = socket;
        _type = type;
    }

    /**
     * @brief 返回 socket 文件描述符
     * 
     * @return int 
     */
    int Socket()
    {
        return _socket;
    }

    /**
     * @brief 返回接收缓冲区
     * 
     * @return char* buf缓冲区
     */
    char * Buf()
    {
        return buf;
    }

    int BufSize()
    {
        return sizeof(buf);
    }


    /**
     * @brief 返回socket类型，如果是交个我托管时候没有设置，请不要相信我的返回值
     * 
     * @return std::string 类型，不是“tcp”就是“udp”
     */
    std::string Type()
    {
        return _type;
    }

    /**
     * @brief 把 buf缓冲区清空
     * 
     */
    void ClearBuf()
    {
        memset(buf, 0, sizeof(buf));
    }


    /**
     * @brief 作为服务端第一步，绑定本机端口，默认监听所有网卡（0.0.0.0）
     * 
     * @param port 端口号
     * @return int 
     */
    int Bind(int port)
    {
        
        struct sockaddr_in serverAddr;
        memset(&serverAddr, 0, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        serverAddr.sin_port = htons(port);

        return bind(_socket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));        
    }
    
//--------------------------------------------
// 服务端专用函数

    /**
     * @brief 作为服务端第二步，设置成监听模式
     * 
     * @return int 同 listen 函数，成功 0，失败 -1
     */
    int Listen()
    {
        return listen(_socket, 20);
    }

    /**
     * @brief 作为服务端第二步，设置成监听模式
     * 
     * @param num num个请求可以等待连接
     * 
     * @return int 同 listen 函数，成功 0，失败 -1
     */
    int Listen(int num)
    {
        return listen(_socket, num);
    }


    /**
     * @brief udp不需要此函数直接Recv！阻塞等待tcp客户端连接，这会产生一个和客户端连接的新的socket，请你保存好 
     * 你也可以使用 new CSocket(返回的int) 来把这个socket交给我的其它对象管理
     * 
     * @return int 返回客户端socket
     */
    int Accept()
    {
        socklen_t clntAddrSize = sizeof(clntAddr); // 结构体大小

        // inet_ntoa(clntAddr.sin_addr); // 客户端ip
        
        // printf("客户端（%s）已连接。\n",inet_ntoa(clntAddr.sin_addr));

        return accept(_socket, (struct sockaddr*)&clntAddr, &clntAddrSize);
        
    }

    /**
     * @brief 直接初始化作为tcp服务端socket，直接等待客户端连接
     * 
     * @param port 监听的端口号
     * 
     * @return int 连接到客户端的socket。Bind或Listen失败则返回-1，未考虑Accept失败
     */
    int S_Init(int port)
    {
        if (Bind(port) == 0 && Listen() == 0)
        {
            // socket监听成功
            return Accept();
        }
        return -1;
    }



//---------------------------------------------



//---------------------------------------------
// 客户端专用函数

    /**
     * @brief 连接服务端
     * 
     * @param ip 服务器地址
     * @param port 服务器开放连接的端口号
     * @return int 同 connect 函数，成功 0 ，失败 -1
     */
    int Connect(const char * ip, int port)
    {
        struct sockaddr_in serverAddr;
        memset(&serverAddr, 0, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = inet_addr(ip);
        serverAddr.sin_port = htons(port);

        return connect(_socket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    }


//---------------------------------------------


    /**
     * @brief 通过 socket 发送消息
     * 
     * @param buffer 要发送的字符串指针
     * @return int 同 send 函数，成功 发送大小，失败 -1
     */
    int Send(char *buffer)
    {
        int ret = send(_socket, buffer, strlen(buffer), 0);
        if(ret == -1)
        {
            std::cerr << "socket发送错误\n";
        }
        return ret;
        
    }


    /**
     * @brief 格式化发送字符串，需要用到buf，请保证分配的buf足够大
     * 
     * @return int 成功 发送大小，失败 -1
     */
    int Send(const char *__restrict __fmt, ...)
    {
        va_list ap;
        va_start (ap, __fmt);
        ClearBuf();
        if(vsprintf(buf, __fmt, ap) < 0)
            return -1;
        int ret = Send(buf);
        va_end (ap);

        return ret;
    }


    /**
     * @brief 通过 socket 发送数据
     * 
     * @param buffer 要发送的数据指针
     * @param len 数据大小
     * @return int 同 send 函数
     */
    int Send(void *buffer, size_t len)
    {
        int ret = send(_socket, buffer, len, 0);
        if(ret == -1)
        {
            std::cerr << "socket发送错误\n";
        }
        return ret;
        
    }



    /**
     * @brief 通过 socket 阻塞接收服务器消息，若对方断开则会马上返回0
     * 
     * @return int 接收的字符串数量
     */
    int Recv()
    {
        ClearBuf();
        int bytes = recv(_socket, buf, sizeof(buf), 0);
        if ( bytes < 0)
        {
            perror("接收recv函数错误");
        }
        else if ( bytes == 0 )
        {
            if (IsConnect()) printf("断开了连接\n");
        }
        return bytes;
    }


    /**
     * @brief 同 Recv，只是返回的是连接客户端信息。
     * 可以使用：inet_ntoa(函数返回->sin_addr) 获取连接地址字符串
     * 
     * @return struct sockaddr_in* 连接的地址，如果接收到0bytes或者运行错误返回NULL
     */
    struct sockaddr_in* RecvFrom()
    {
        ClearBuf();

        memset(&clntAddr, 0, sizeof(clntAddr));
        socklen_t clntAddrLen = sizeof(clntAddr);

        int bytes = recvfrom(_socket, buf, sizeof(buf), 0, (struct sockaddr *)&clntAddr, &clntAddrLen);
        // 判断接收
        if ( bytes < 0 )
        {
            perror("接收recv函数错误");
            return NULL;
        }
        else if ( bytes == 0 )
        {
            if (IsConnect()) printf("断开了连接\n");
            return NULL;
        }

        return &clntAddr;
    }



    /**
     * @brief 查看 socket 是否还连接着
     * 
     * @return true 未断开
     * @return false 已断开
     */
    bool IsConnect()
    {
        struct tcp_info info; 
        int len=sizeof(info); 
        getsockopt(_socket, IPPROTO_TCP, TCP_INFO, &info, (socklen_t *)&len);
        if(info.tcpi_state == TCP_ESTABLISHED) return true;
        else return false;
    }
    

    /**
     * @brief 作为服务端关闭用于监听的socket，作为客户端关闭与服务端的连接
     * 
     * @return int 同 close 函数
     */
    int Close()
    {
        return close(_socket);
    }


};


#endif
