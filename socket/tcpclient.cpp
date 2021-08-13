#include "socket.hpp"


void tcpClientExp(char **argv)
{
    class TcpClient *tcp = new TcpClient();
    std::string str;

    tcp->Connect(argv[1], atoi(argv[2]));

    while (true)
    {
        std::cout << "input:";
        std::cin >> str;
        if (str == "q")
            break;
        tcp->Send((void*)str.c_str(), str.length());
        std::cout << "send:" << str << std::endl;
    }
    
    tcp->Close();

    delete tcp;
}


int main(int argc, char *argv[])
{
    tcpClientExp(argv);
    return 0;
}
