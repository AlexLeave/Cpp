/**
 * @file sem.hpp
 * @author csu-lf (csu.lifeng@qq.com)
 * @brief 封装了常用的二元信号基本用法，注意不是所有信号集！！！专门用于多进程共享内存实现锁等功能
 * @version 0.1
 * @date 2021-07-04
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#ifndef _MY_SEM
#define _MY_SEM 1


#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <stdio.h>


// 用于信号灯操作的共同体。
union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *arry;
};



class CSem
{
    private:
    int semid;
    key_t key;
    int _Authority; // 信号的权限，默认所有用户可读可写


    /**
     * @brief 创建和初始化二元信号
     * 
     * @param bool 是否创建和初始化成功
     */
    bool Init()
    {
        // 不存在，创建
        if ( (semid = semget(key, 1, _Authority)) == -1 && errno == ENOENT)
        {        
            if ( (semid = semget(key, 1, IPC_CREAT|_Authority)) == -1)
            {
                perror("erro:");
                return false;
            }
            else
            {
                // 初始化，把信号设置成可用（unlock）状态
                union semun sem_union;
                sem_union.val = 1; // 1 表示可用状态
                semctl(semid,0,SETVAL,sem_union);
            }
            
        }
        else
        {
            // printf("信号已存在\n");// 存在
        }

        return true;
    }


    public:
    /**
     * @brief 创建一个二元信号
     * 
     * @param Key 信号的key
     * @param authority 信号权限
     */
    CSem(key_t Key = 0x10086, int authority = 0640)
    {
        key = Key;
        _Authority = authority;
        if(Init() == false)
        {
            printf("信号创建失败\n");
        }
    }


    int Semid()
    {
        return semid;
    }
    
    int Authority()
    {
        return _Authority;
    }


    /**
     * @brief 申请使用资源，给信号上锁。若是已锁，则等待使用资源的进程给信号解锁
     * 
     * @return int 使用函数 semop 上锁的返回值
     */
    int Lock()
    {
        struct sembuf sem_b;
        sem_b.sem_num = 0;
        sem_b.sem_op = -1;
        sem_b.sem_flg = SEM_UNDO;
        return semop(semid, &sem_b, 1);
    }

    /**
     * @brief 使用资源的进程给信号解锁，释放资源
     * 
     * @return int 使用函数 semop 解锁的返回值
     */
    int UnLock()
    {
        struct sembuf sem_b;
        sem_b.sem_num = 0;
        sem_b.sem_op = 1;
        sem_b.sem_flg = SEM_UNDO;
        return semop(semid, &sem_b, 1);
    }

    /**
     * @brief 删除信号
     * 
     * @return int 使用 semctl 函数删除信号的返回值
     */
    int Destory()
    {
        return semctl(semid, 0, IPC_RMID);
    }


};




#endif