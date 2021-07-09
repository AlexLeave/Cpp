/**
 * @file sever.cpp
 * @author csu-lf (csu.lifeng@qq.com)
 * @brief socket.hpp 测试文件，作为服务端，特别是多线程应用测试，查看CSocket对多线程编程兼容性如何
 * @version 0.1
 * @date 2021-07-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "socket.hpp"
#include <thread>


int thread_main();


class CSocket *server_socket = new CSocket("tcp");

int main(int argc, char const *argv[])
{
    

    server_socket->Bind(1010);
    server_socket->Listen();

    std::thread *my_thread = new std::thread(thread_main);
    // my_thread->join();
    // thread_main();
    while(1);

    server_socket->Close();

    return 0;
}

int thread_main()
{
    printf("new thread\n");
    class CSocket *client_socket = new CSocket(server_socket->Accept());
    printf("link\n");
    std::thread *my_thread_child = new std::thread(thread_main);
    if(client_socket->Socket() < 0)
    {
        printf("accept erro\n");
        printf("thread exit\n");
        return -1;
    }
    while(client_socket->Recv() >= 0 && client_socket->IsConnect())
    {
        printf("recv: %s\n", client_socket->Buf());
    }
    client_socket->Close();
    printf("thread exit\n");

    return 0;
}






