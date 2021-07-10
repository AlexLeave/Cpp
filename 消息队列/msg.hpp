/**
 * @file msg.hpp
 * @author csu-lf (csu.lifeng@qq.com)
 * @brief 封装消息队列，类真是太好用啦~！你说如果有更重要信息想先拿这个实现不了？你多创建一个消息队列不就行了
 * @version 0.1
 * @date 2021-07-06
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _MY_MSG
#define _MY_MSG 1

#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <sys/msg.h>


#define _MY_MSG_MAXLEN 512 // 消息队列发送数据最大值
#define _MY_MSG_TYPE 10 // 用于验证接收和发送的消息类型，必须大于0

struct my_msgbuf {
               long mtype;       // 消息的类型，别设置成0！！！！！
               char mtext[_MY_MSG_MAXLEN];    // 真实消息内容
           };



class CMsg
{
    private:
    int _msg_id;
    key_t _key; // 就是 int 类型
    int _authority = 0666; // 消息队列权限
    struct my_msgbuf _buf;


    public:
    /**
     * @brief 根据 key 查找是否存在消息队列，没有就创建。成功获取消息队列那么_msg_id非负
     * 且消息队列读取不阻塞
     * 
     * @param key 最好写十六进制
     * @param authority 最好写八进制，例如：0666
     */
    CMsg(int key = 0x50, int authority = 0666)
    {
        _key = key;
        _authority = authority;
        _msg_id = msgget(_key, _authority|IPC_CREAT|IPC_NOWAIT);
    }

    /**
     * @brief 清空消息缓冲区buf
     * 
     */
    void Clear()
    {
        memset(&_buf, 0, sizeof(_buf));
    }

    /**
     * @brief 返回缓冲区buf
     * 
     * @return char* 
     */
    char *Buf()
    {
        return _buf.mtext;
    }

    /**
     * @brief 获取 msgid
     * 
     * @return int msgid
     */
    int Msgid()
    {
        return _msg_id;
    }


    /**
     * @brief 向消息队列发送消息
     * 
     * @param str 发送的消息
     * @return true 发送成功
     * @return false 发送失败
     */
    bool Write(char *str)
    {
        Clear();
        _buf.mtype = _MY_MSG_TYPE;
        sprintf(_buf.mtext, "%s", str);
        if(msgsnd(_msg_id, &_buf, _MY_MSG_MAXLEN, 0) < 0) return false;
        return true;
    }

    /**
     * @brief 读取消息队列消息，读取内容保存在 buf
     * 
     * @return true 读取成功
     * @return false 读取失败
     */
    bool Read()
    {
        Clear();
        if(msgrcv(_msg_id, &_buf, _MY_MSG_MAXLEN, _MY_MSG_TYPE, 0)<0) return false;
        return true;
    }

    /**
     * @brief 删除消息队列
     * 
     * @return true 删除成功
     * @return false 删除失败
     */
    bool Destory()
    {
        if(msgctl(_msg_id, IPC_RMID, NULL) == 0) return true;
        return false;
    }



};






#endif

