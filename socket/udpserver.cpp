#include <stdio.h>
#include "socket.hpp"

void test_udpserver(char** argv)
{
    class UdpServer *udpserver = new UdpServer();
    if (udpserver->Bind(atoi(argv[1])))
    {
        perror("bind");
        return;
    }

    printf("bind succeed\n");
    printf("start recv from %d\n", udpserver->Socket());
    for (int i = 0; i < 10; i++)
    {
        udpserver->Recv();
        printf("recv from %s:%d : %s\n",  udpserver->ClientIP(), 
                    udpserver->ClientPort(), (char*)udpserver->Buf());
        fflush(stdout);
    }

    udpserver->Close();
    
    delete udpserver;
}


int main(int argc, char *argv[])
{
    // 使用示例：./udpserver 10086
    // 解释：后面的10086表示监听10086端口
    if (argc < 2)
        return -1;
    

    test_udpserver(argv);
    
    return 0;
}



