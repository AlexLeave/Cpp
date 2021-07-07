#include "share_mem.hpp"
#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{
    class CShmem *mem = new CShmem();
    char p[10];

    mem->erase_shmem();
    cin >> p;
    sprintf((char*)mem->get_p(), "%s\n", p);
    printf("%s", (char *)mem->get_p());

    return 0;
}







