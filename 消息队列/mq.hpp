/**
 * @file mq.hpp
 * @author csu-lf (csu.lifeng@qq.com)
 * @brief POSIX消息队列，相比于msg.hpp更加通用，使用起来很方便的
 * @version 0.1
 * @date 2021-07-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#ifndef _MY_MQ_H
#define _MY_MQ_H 1


#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>



class CMq
{
    private:
    int _mqid;



    public:
    /**
     * @brief 创建消息队列
     * 
     * @param name Posix IPC名字一般以斜杠“/”作为首字母，且名字中只有一个“/”，例如"/test_mq"
     * @param oflag 有必须的选项：O_RDONLY，O_WRONLY，O_RDWR，还有可选的选项：O_NONBLOCK，O_CREAT，O_EXCL
     * @param mode 权限，例如：0640
     * @param attr NULL就采用默认属性，
     */
    CMq(const char *name, int oflag, mode_t mode, struct mq_attr *attr)
    {
        _mqid = mq_open(name, oflag, mode, attr);

    }




};




#endif
