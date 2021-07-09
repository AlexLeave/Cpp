/**
 * @file socket.hpp
 * @author csu-lf (csu.lifeng@qq.com)
 * @brief 封装了常用的网络编程的socket成CSocket类，请愉快的使用吧
 * @version 0.1
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

#include "poll.hpp"


/**
 * @brief 使用C++类封装socket，创建socket只需要创建这个类，服务端socket使用server开头命名的函数
 * 客户端socket使用client开头命名的函数
 * 
 */
class CSocket{
    private:
    int _socket;
    char buf[5*1024]; // 测试接收web页面专用大小
    struct sockaddr_in clntAddr; //结构体，存放客户端socket信息
    int _socket_client; // 保存服务端监听到的客户端socket
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

    int get_socket()
    {
        return _socket;
    }

    char * get_buf()
    {
        return buf;
    }

    /**
     * @brief 把 buf缓冲区清空
     * 
     */
    void clear_buf()
    {
        memset(buf, 0, sizeof(buf));
    }
//--------------------------------------------
// 服务端专用函数

    /**
     * @brief 绑定本机端口，默认监听所有网卡（0.0.0.0）
     * 
     * @param port 端口号
     * @return int 
     */
    int server_bind(int port)
    {
        
        struct sockaddr_in serverAddr;
        memset(&serverAddr, 0, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        serverAddr.sin_port = htons(port);

        return bind(_socket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));        
    }

    /**
     * @brief 设置成监听模式
     * 
     * @return int 同 listen 函数
     */
    int server_listen()
    {
        return listen(_socket, 20);
        
    }

    /**
     * @brief 阻塞等待客户端连接
     * 
     * @return int 返回客户端socket
     */
    int server_accept()
    {
        socklen_t clntAddrSize = sizeof(clntAddr); // 结构体大小

        // inet_ntoa(clntAddr.sin_addr); // 客户端ip
        
        // printf("客户端（%s）已连接。\n",inet_ntoa(clntAddr.sin_addr));

        return _socket_client = accept(_socket, (struct sockaddr*)&clntAddr, &clntAddrSize);
        
    }

    /**
     * @brief 直接初始化服务端socket，直接就可以让socket发送和接收了
     * 
     * @param port 监听的端口号
     */
    bool server_init(int port)
    {
        if (server_bind(port) == 0 && server_listen() == 0)
        {
            // socket监听成功
            if(server_accept() >= 0) return true;
        }
        return false;
    }


    /**
     * @brief 接收客户端消息
     * 
     * @return char* 客户端发过来的消息，最大5KB字符
     */
    char* server_recv()
    {
        
        clear_buf();
        if (recv(_socket_client, buf, sizeof(buf), 0) < 0)
        {
            perror("recv错误：\n");
        }
        // printf("%s", buf);
        return buf;
    }

    /**
     * @brief 发送数据给客户端
     * 
     * @param buffer 要发送的数据
     * @return int 同 send 函数
     */
    int server_send(char* buffer)
    {
        return send(_socket_client, buffer, strlen(buf), 0);
        
    }

    /**
     * @brief 查看 socket 是否还连接着
     * 
     * @return true 未断开
     * @return false 已断开
     */
    bool is_connect_server()
    {
        struct tcp_info info; 
        int len=sizeof(info); 
        if (getsockopt(_socket_client, IPPROTO_TCP, TCP_INFO, &info, (socklen_t *)&len) < 0) 
            return false;
        if(info.tcpi_state == TCP_ESTABLISHED) return true;
        else return false;
    }



//---------------------------------------------



//---------------------------------------------
// 客户端专用函数

    /**
     * @brief 连接服务端
     * 
     * @param ip 服务器地址
     * @param port 服务器开放连接的端口号
     * @return int 同 connect 函数
     */
    int client_connect(const char * ip, int port)
    {
        struct sockaddr_in serverAddr;
        memset(&serverAddr, 0, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = inet_addr(ip);
        serverAddr.sin_port = htons(port);

        return connect(_socket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    }

    /**
     * @brief 发送消息给服务器
     * 
     * @param buffer 要发送的数据
     * @return int 同 send 函数
     */
    int client_send(char *buffer)
    {
        int ret = send(_socket, buffer, strlen(buffer), 0);
        if(ret == -1)
        {
            std::cerr << "socket发送错误\n";
        }
        return ret;
        
    }

    /**
     * @brief 接收服务器消息
     * 
     * @return int 接收的字符串数量
     */
    int client_recv()
    {
        clear_buf();
        int bytes = recv(_socket, buf, sizeof(buf), 0);
        if ( bytes < 0)
        {
            perror("接收recv函数错误：\n");
        }
        else if ( bytes == 0 )
        {
            if (is_connect_client()) printf("断开了连接\n");
        }
        return bytes;
    }

    /**
     * @brief 查看 socket 是否还连接着
     * 
     * @return true 未断开
     * @return false 已断开
     */
    bool is_connect_client()
    {
        struct tcp_info info; 
        int len=sizeof(info); 
        getsockopt(_socket, IPPROTO_TCP, TCP_INFO, &info, (socklen_t *)&len);
        if(info.tcpi_state == TCP_ESTABLISHED) return true;
        else return false;
    }
    

//---------------------------------------------

    /**
     * @brief 作为服务端关闭用于监听的socket，作为客户端关闭与服务端的连接
     * 
     * @return int 同 close 函数
     */
    int socket_close()
    {
        return close(_socket);
    }

    /**
     * @brief 服务端关闭和客户端连接的socket
     * 
     * @return int 使用 close函数 返回
     */
    int socket_close_server()
    {
        return close(_socket_client);
    }




};


#endif