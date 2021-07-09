/**
 * @file lock.hpp
 * @author csu-lf (csu.lifeng@qq.com)
 * @brief 多线程工具，防止数据被不同线程同时使用的互斥锁工具（原本函数名太难记了，封装一下）
 * @version 0.1
 * @date 2021-07-04
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _MY_LOCK
#define _MY_LOCK 1

#include <pthread.h>
#include <stdio.h>

enum LockType
{
    block = 0,
    unblock = 1
};


class CLock
{
    private:
    pthread_mutex_t mutex;

    int init_lock()
    {
        return pthread_mutex_init(&mutex, NULL);
    }

    public:
    /**
     * @brief 创建一个默认属性的互斥锁，C++11 也有自己的互斥锁类 class mutex #include <mutex>，蛮好用的
     * 
     */
    CLock()
    {
        init_lock();
    }

    pthread_mutex_t get_mutex();

    /**
     * @brief 给这个锁上锁
     * 
     * @param type 上锁类型，block代表阻塞，unblock代表非阻塞
     * @return int 成功返回 0
     */
    int lock(LockType type);

    /**
     * @brief 解锁，释放资源
     * 
     * @return int 成功返回 0
     */
    int unlock();




};

pthread_mutex_t CLock::get_mutex()
{
    return mutex;
}

int CLock::lock(LockType type)
    {
        if(type == block)
        {
            return pthread_mutex_lock(&mutex);
        }
        else if(type == unblock)
        {
            return pthread_mutex_trylock(&mutex);
        }
        else
        {
            printf("未知的参数错误\n");
            return -1;
        }
    }


int CLock::unlock()
{
    return pthread_mutex_unlock(&mutex);
}


#endif 
