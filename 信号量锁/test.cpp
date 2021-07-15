/**
 * @file test.cpp
 * @author csu-lf (csu.lifeng@qq.com)
 * @brief 运行两个此程序检测信号量锁是否成功，注意使用完成后ipcrm sem shmid 删除信号量
 * @version 0.1
 * @date 2021-07-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#include "sem.hpp"
#include <iostream>
#include <unistd.h>

using namespace std;


int main(int argc, char const *argv[])
{
    class CSem *sem = new CSem(0x1001);

    cout << getpid() << endl;
    sem->Lock();
    cout << "使用中" << endl;


    sem->UnLock();
    cout << "用完了\n";

    // sem->Destory();
    delete sem;


    return 0;
}





