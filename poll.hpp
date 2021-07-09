/**
 * @file poll.hpp
 * @author csu-lf (csu.lifeng@qq.com)
 * @brief 把 poll函数 封装成类，一个文件流创建一个类对象就能判断文件状态
 * @version 0.1
 * @date 2021-07-07
 * 
 * @copyright Copyright (c) 2021
 * 
 */



#ifndef _MY_POLL_H
#define _MY_POLL_H 1

#include <signal.h>
#include <poll.h>
#include <iostream>
#include <stdio.h>


// 可读状态
#define POLL_READ (POLLIN | POLLPRI)
// 错误状态
#define POLL_ERROR (POLLERR | POLLHUP | POLLNVAL)
// 可写状态
#define POLL_WRITE (POLLOUT)



class CPoll
{
    private:
    unsigned long int _nfds = 1; // number of fd struct
    struct pollfd  _fds; // fd struct 
    int _timeout = 0; // 默认即刻应答
    // int poll(struct pollfd *fds, nfds_t nfds, int timeout);


    public:
    /**
     * @brief 创建一个检测文件流是否可读，可写
     * 
     * @param fd 文件流标识符
     */
    CPoll(int fd)
    {
        _fds.fd = fd;
    }

    /**
     * @brief 设置超时时间，即阻塞这么多时间后才响应
     * 
     * @param timeout 
     * @return int 超时时间
     */
    int set_timeout(int timeout)
    {
        _timeout = timeout;
        return _timeout;
    }

    /**
     * @brief 获取超时时间
     * 
     * @return int 超时时间
     */
    int get_timeout()
    {
        return _timeout;
    }

    /**
     * @brief 获取 poll函数 需要的的文件流结构体指针
     * 
     * @return struct pollfd* 
     */
    struct pollfd* get_fds()
    {
        return &_fds;
    }


    /**
     * @brief 判断获取文件流当前状态
     * 
     * @param events POLL_READ 是否可读 POLL_WRITE 是否可写 POLL_ERROR 是否错误状态
     * @return true 是 可读/可写/错误
     * @return false 不是 可读/可写/错误
     */
    bool check(short int events)
    {
        _fds.events = events;
        _fds.revents = 0;
        if(poll(&_fds, _nfds, _timeout) < 0)
        {
            perror("poll函数错误：\n");
        }
        if (_fds.revents & events) return true;
        else return false;
    }




};


#endif

