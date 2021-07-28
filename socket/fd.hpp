/**
 * @file fd.hpp
 * @author csu-lf (csu.lifeng@qq.com)
 * @brief 封装一个管理文件描述符的class，同时封装进了poll函数
 * poll.hpp升级版
 * 
 * @version 1.1
 * @date 2021-07-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _MY_FD_H
#define _MY_FD_H 1

#ifndef POLL_READ
// 可读状态
#define POLL_READ (POLLIN | POLLPRI)
#endif
#ifndef POLL_ERROR
// 错误状态
#define POLL_ERROR (POLLERR | POLLHUP | POLLNVAL)
#endif
#ifndef POLL_WRITE
// 可写状态
#define POLL_WRITE (POLLOUT)
#endif

#include <stdio.h>
#include <signal.h>
#include <poll.h>
#include <iostream>

class CFd
{
    private:
    int _len = 0; // 初始内部不存在文件描述符
    int *fds; // 保存文件描述符
    int _max_len; // 最大的管理数量

    public:
    /**
     * @brief 创建文件描述符管理类，创建失败调用 Len() 返回 -1
     * 
     * @param max_len 你需要管理文件描述符的最大数量
     */
    CFd(int max_len)
    {
        fds = new (std::nothrow) int[max_len];
        _max_len = max_len;
        if (fds == NULL)
        {
            _max_len = 0;
            _len = -1;
        }
        
    }

    /**
     * @brief 找文件描述符在不在里面，在哪里
     * 
     * @param fd 你要找的文件描述符
     * @return int 存在 返回在里面的位置（index），不存在 -1
     */
    int Find(int fd)
    {
        int i = 0;
        for(i = 0; i < _len; i++)
        {
            if( fds[i] == fd ) return i;
        }
        return -1;
    }

    /**
     * @brief 删除一个我管理的文件描述符
     * 
     * @param fd 你要删除的文件描述符
     * @return true 删除成功
     * @return false 删除失败
     */
    bool Delete(int fd)
    {
        int index = Find(fd);
        if( index == -1 ) 
        {
            // printf("文件描述符不存在\n");
            return false; // 不存在
        }
        else
        {
            fds[index] = fds[_len - 1];
            _len--;
            return true;
        }
    }

    /**
     * @brief 添加一个文件描述符给我管理，小于零会失败
     * 
     * @param fd 文件描述符
     * @return true 添加成功
     * @return false 已满，添加失败
     */
    bool Add(int fd)
    {
        if (Find(fd) >= 0 || fd < 0)
        {
            // printf("已存在文件描述符或文件描述符错误（小于零）\n");
            return false;
        }
        if(_len < _max_len)
        {
            fds[_len] = fd;
            _len++;
            return true;
        }
        else
        {
            return false;
        }
        
    }


    /**
     * @brief 返回里面管理的文件描述符个数
     * 
     * @return int 
     */
    int Len(){ return _len; }


    /**
     * @brief 传入index，返回文件描述符
     * 
     * @param index index
     * @return int 文件描述符
     */
    int Fd(int index)
    {
        return fds[index];
    }


    /**
     * @brief 判断文件流当前状态，返回当前所有文件流里面是否有想要的状态
     * 
     * @param events POLL_READ 是否可读 POLL_WRITE 是否可写 POLL_ERROR 是否错误状态
     * @param _timeout 超时时间，等到这么久（单位秒）后才判断返回
     * 
     * @return int 成功 存在几个是events状态，失败 -1
     */
    int Poll(short int events, int _timeout)
    {
        struct pollfd*  _fds = new (std::nothrow) struct pollfd[Len()]; // poll函数需要的关于文件描述符的结构体 
        
        if (_fds == NULL)
        {
            printf("内存不够啦\n");
            return -1;
        }
        
        for (int i = 0; i < Len(); i++)
        {
            _fds[i].fd = fds[i];
            _fds[i].events = events;
            _fds[i].revents = 0;
        }
        int for_return = poll(_fds, Len(), _timeout);

        delete[] _fds;
        return for_return;
        
    }


    /**
     * @brief 判断文件流当前状态，返回当前所选文件流里面是否有想要的状态
     * 
     * @param index 按照index选择类里面的文件标识符来判断
     * @param events POLL_READ 是否可读 POLL_WRITE 是否可写 POLL_ERROR 是否错误状态
     * @param _timeout 超时时间，等到这么久（单位秒）后才判断返回，-1表示永久等待
     * 
     * @return true 返回事件与请求事件一致
     * @return false 返回事件与请求事件不一致
     */
    bool Poll(int index, short int events, int _timeout)
    {
        struct pollfd*  _fds = new (std::nothrow) struct pollfd; // poll函数需要的关于文件描述符的结构体 
        
        if (_fds == NULL)
        {
            printf("内存不够啦\n");
            return -1;
        }
        
        _fds->fd = fds[index];
        _fds->events = events;
        _fds->revents = 0;
        bool for_return;
        if(poll(_fds, 1, _timeout) == 1 && _fds->revents == events) // poll返回的事件和请求事件一致
            for_return = true;
        else
            for_return = false;

        delete[] _fds;
        return for_return;
    }


    /**
     * @brief 找一找文件流里面谁可以触发events事件
     * 
     * @param events POLL_READ 可读 POLL_WRITE 可写 POLL_ERROR 错误状态
     * @return int 存在 返回一个可以触发事件的文件标识符的index，不存在 返回-1
     */
    int Who(short int events)
    {
        for(int i = 0; i < Len(); i++)
        {
            if (Poll(i, events, 0))
                return i;
        }
        return -1;
    }



};


#endif
