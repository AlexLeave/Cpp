/**
 * @file test.cpp
 * @author csu-lf (csu-lf@qq.com)
 * @brief 测试 fd.cpp 管理文件描述符是否实现
 * @version 0.1
 * @date 2021-07-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "fd.hpp"
#include <iostream>

int main(int argc, char const *argv[])
{
    class CFd *fds = new class CFd(10);

    fds->Add(10);
    fds->Add(9);
    fds->Add(8);
    fds->Add(7);
    fds->Add(6);
    fds->Add(5);
    fds->Add(4);
    fds->Add(3);
    fds->Delete(3);
    fds->Add(2);
    fds->Add(1);
    fds->Add(11);
    fds->Add(12);
    fds->Add(20);

    std::cout << fds->Len();


    return 0;
}



