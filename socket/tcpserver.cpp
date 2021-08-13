#include "socket.hpp"


void tcpServerExp(char **argv)
{
    class TcpServer *tcp = new TcpServer();
    
    if (tcp->Bind(atoi(argv[1])))
    {
        perror("bind");
    }

    if (!tcp->Listen())
    {
        printf("listen succeed\n");
    }
    class Tcp *tcp_connect = new Tcp(tcp->Accept());
    printf("socket:%d\n", tcp_connect->Socket());

    tcp_connect->Recv();
    printf("recv:%s\n", (char*)tcp_connect->Buf());

    tcp_connect->Close();
    tcp->Close();

    delete tcp_connect;
    delete tcp;
}



int main(int argc, char *argv[])
{
    tcpServerExp(argv);
    return 0;
}



