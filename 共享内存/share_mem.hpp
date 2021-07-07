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
    int shmid; // 共享内存id
    key_t key; // 共享内存键值
    size_t size; // 共享内存大小
    int authority; // 共享内存权限
    void *p; // 指向共享内存第一个地址的指针

    bool init_shmem()
    {
        // 创建共享内存
        shmid = shmget(key, size, authority|IPC_CREAT);
        if(shmid == -1) // 创建失败
        {
            perror("共享内存创建失败:");
            return false;
        }
        // 连接共享内存到当前进程地址空间
        p = shmat(shmid, 0, 0);
        if(p == (void *) -1) // 连接失败
        {
            perror("共享内存连接到当前进程地址空间失败:");
            return false;
        }

        return true;
    }


    public:
    /**
     * @brief 创建一块共享内存，默认1k大小
     * 
     * @param Key 共享内存键值，默认 0x5005，请使用十六进制
     * @param Size 共享内存大小
     * @param Authority 共享内存权限，默认0640，请使用八进制
     */
    CShmem(key_t Key = 0x5005, size_t Size = 1024, int Authority = 0640) // 默认创建 1k 大小共享内存
    {
        key = Key;
        size = Size;
        authority = Authority;
        init_shmem();
    }

    int get_shmid()/* code */
    {
        return key;
    }

    size_t get_size()
    {
        return size;
    }

    int get_authority()
    {
        return authority;
    }

    void* get_p()
    {
        return p;
    }

    /**
     * @brief 擦除共享内存内容
     * 
     * @return void* 返回共享内存首地址
     */
    void* erase_shmem()
    {
        return memset(p, 0, size);
    }


    /**
     * @brief 把共享内存从当前进程分离
     * 
     * @return true 分离成功
     * @return false 分离失败
     */
    bool depart_shmem()
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
    bool delete_shmem()
    {
        if (shmctl(shmid, IPC_RMID, 0) == -1) // 删除共享内存
        {
            return false;
        }
        else return true;
    }

};


#endif