/**
 * @file share_mem.hpp
 * @author csu-lf (csu.lifeng@qq.com)
 * @brief 封装共享内存基本操作，请愉快的使用C++带来的类吧~~~~，C++ yyds
 * @version 0.1
 * @date 2021-07-04
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _SHMEM
#define _SHMEM 1


#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

class CShmem
{
    private:
    int _shmid; // 共享内存id
    key_t _key; // 共享内存键值
    size_t _size; // 共享内存大小
    int _authority; // 共享内存权限
    void *p; // 指向共享内存第一个地址的指针

    bool Init()
    {
        // 创建共享内存
        _shmid = shmget(_key, _size, _authority|IPC_CREAT);
        if(_shmid == -1) // 创建失败
        {
            perror("共享内存创建失败:");
            p = NULL;
            return false;
        }
        // 连接共享内存到当前进程地址空间
        p = shmat(_shmid, 0, 0);
        if(p == (void *) -1) // 连接失败
        {
            perror("共享内存连接到当前进程地址空间失败:");
            p = NULL;
            return false;
        }

        return true;
    }


    public:
    /**
     * @brief 创建一块共享内存，默认1k大小，如果有相同_key共享内存已创建则返回它，创建失败共享内存指针为 NULL
     * 
     * @param key 共享内存键值，默认 0x5005，请使用十六进制
     * @param size 共享内存大小
     * @param authority 共享内存权限，默认0640，请使用八进制
     */
    CShmem(key_t key = 0x5005, size_t size = 1024, int authority = 0640) // 默认创建 1k 大小共享内存
    {
        _key = key;
        _size = size;
        _authority = authority;
        Init();
    }

    int Shmid()
    {
        return _shmid;
    }

    size_t Size()
    {
        return _size;
    }

    int Authority()
    {
        return _authority;
    }

    /**
     * @brief 返回共享内存首地址指针
     * 
     * @return void* 共享内存首地址指针
     */
    void* Ptr()
    {
        return p;
    }

    /**
     * @brief 擦除共享内存内容
     * 
     * @return void* 
     */
    void* Erase()
    {
        return memset(p, 0, _size);
    }


    /**
     * @brief 把共享内存从当前进程分离
     * 
     * @return true 分离成功
     * @return false 分离失败
     */
    bool Depart()
    {
        if (shmdt(p) == -1) // 分离共享内存
        {
            return false;
        }
        else return true;
    }

    /**
     * @brief 删除共享内存
     * 
     * @return true 删除成功
     * @return false 删除失败
     */
    bool Destory()
    {
        if (shmctl(_shmid, IPC_RMID, 0) == -1) // 删除共享内存
        {
            return false;
        }
        else return true;
    }

};


#endif