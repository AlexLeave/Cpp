/**
 * @file child_socket.hpp
 * @author csu-lf (csu.lifeng@qq.com)
 * @brief 面向服务端解决多线程socket问题，把与用户连接的socket给此类委托管理即可
 * @version 0.1
 * @date 2021-07-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _MY_CHILD_SOCKET_H_
#define _MY_CHILD_SOCKET_H_ 1


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


class ChildSocket
{
    private:
    int _socket; // 需要托管的socket
    char buf[5*1024]; // 测试接收web页面专用大小


    public:
    /**
     * @brief 把服务端和客户端连接的socket传进来
     * 
     * @param client_socket 与客户端连接的socket
     */
    ChildSocket(int client_socket)
    {
        _socket = client_socket;
    }
    
    /**
     * @brief 返回与客户端连接的socket
     * 
     * @return int 
     */
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

    /**
     * @brief 接收客户端消息
     * 
     * @return char* 客户端发过来的消息，最大5KB字符
     */
    char* server_recv()
    {
        
        clear_buf();
        if (recv(_socket, buf, sizeof(buf), 0) < 0)
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
        return send(_socket, buffer, strlen(buf), 0);
        
    }

    /**
     * @brief 关闭与客户端的连接
     * 
     * @return int 使用 close函数 返回
     */
    int destory()
    {
        return close(_socket);
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
        if (getsockopt(_socket, IPPROTO_TCP, TCP_INFO, &info, (socklen_t *)&len) < 0) 
            return false;
        if(info.tcpi_state == TCP_ESTABLISHED) return true;
        else return false;
    }

};







#endif
