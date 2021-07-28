/**
 * @file client.cpp
 * @author csu-lf (csu.lifeng@qq.com)
 * @brief 测试socket.hpp的客户端
 * @version 0.1
 * @date 2021-07-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "socket.hpp"


int main(int argc, char const *argv[])
{
    class CSocket *socket = new CSocket("tcp");

    socket->Bind(40001);
    if (socket->Connect("127.0.0.1", 40005) == -1) return -1;

    std::string str;

    while (true)
    {
        std::cout << "msg: ";
        std::cin >> str;

        if (str == "q") return 0;

        socket->Send((char*)str.c_str());
        std::cout << "send: " << str << std::endl;
    }


    return 0;
}









