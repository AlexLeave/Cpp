/**
 * @file thread.hpp
 * @author csu-lf (csu.lifeng@qq.com)
 * @brief 封装C语言的 pthread.h
 * @version 0.1
 * @date 2021-07-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef _MY_THREAD
#define _MY_THREAD 1

#include <pthread.h>
#include <signal.h>
#include <stdio.h>


class CThread
{
    private:
    pthread_t pthid;

    public:
    /**
     * @brief 创建一个子线程，结束自动释放资源的子线程，C++11 居然有自己的线程类，还挺好用的...能不用这个就别用了
     * C++11 类头文件 #include <thread>
     * 
     * @param start_rtn 子线程函数名
     * @param arg 向子线程函数传递的参数
     */
    CThread(void *(*start_rtn)(void*), void *arg)
    {
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);//设置线程的属性
        pthread_create(&pthid, &attr, start_rtn, arg);
    }

    pthread_t get_pthid()
    {
        return pthid;
    }

    /**
     * @brief 阻塞被动退出子线程，结合pthread_cancel和pthread_join函数实现
     * 
     * @return true 退出成功
     * @return false 退出不成功
     */
    bool thread_cancel()
    {
        if(pthread_cancel(pthid) == 0)
        {
            if(pthread_join(pthid, NULL) == 0) return true;
        }
        
        return false;
    }


    /**
     * @brief 发送信号给子线程，默认发送Interrupt（终止）信号
     * 
     * @param signo 
     * @return true 发送信号成功
     * @return false 发送信号失败
     */
    bool thread_kill(int signo = 2)
    {
        if(pthread_kill(pthid, signo) == 0) return true;
        else
        {
            perror("thread_kill函数执行错误：");
            return false;
        }
    }



};


#endif