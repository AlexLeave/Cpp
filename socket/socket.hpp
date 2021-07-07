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
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string>


/**
 * @brief 使用C++类封装socket，创建socket只需要创建这个类，服务端socket使用server开头命名的函数
 * 客户端socket使用client开头命名的函数
 * 
 */
class CSocket{
    private:
    int _socket;
    char buf[5*1024];


    public:
    /**
     * @brief Construct a new CSocket object
     * 
     * @param type 填“udp”创建udp socket，否则为tcp socket
     */
    CSocket(std::string type = "tcp")
    {
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

    int get_socket(){
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
        return listen(_socket, 10);
        
    }

    /**
     * @brief 阻塞等待客户端连接
     * 
     * @return int 同 accept 函数
     */
    int server_accept()
    {
        struct sockaddr_in clntAddr; //结构体，存放客户端socket信息
        socklen_t clntAddrSize = sizeof(clntAddr); // 结构体大小
        int clntSocket = 0; //客户端socket

        return clntSocket = accept(_socket, (struct sockaddr*)&clntAddr, &clntAddrSize);
        
    }

    /**
     * @brief 直接初始化服务端socket
     * 
     * @param port 监听的端口号
     */
    void server_init(int port)
    {
        server_bind(port);
        server_listen();
        server_accept();
    }


    /**
     * @brief 接收客户端消息
     * 
     * @return char* 客户端发过来的消息，最大5KB字符
     */
    char* server_recv()
    {
        try
        {
            recv(_socket, buf, sizeof(buf), 0);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
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
        return send(_socket, buffer, strlen(buf), 0);
        
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
        return send(_socket, buffer, strlen(buffer), 0);
        
    }

    /**
     * @brief 接收服务器消息
     * 
     * @return char* 接收的消息，最大5KB字符
     */
    char* client_recv()
    {
        try
        {
            recv(_socket, buf, sizeof(buf), 0);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        return buf;
    }
    

//---------------------------------------------

    /**
     * @brief 使用 close 函数关闭 socket
     * 
     * @return int 同 close 函数
     */
    int socket_close()
    {
        return close(_socket);
    }


};


#endif