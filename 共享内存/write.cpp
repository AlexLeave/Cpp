#include "share_mem.hpp"
#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{
    class CShmem *mem = new CShmem();
    char p[10];

    mem->Erase();
    cin >> p;
    sprintf((char*)mem->Ptr(), "%s\n", p);
    printf("%s", (char *)mem->Ptr());

    return 0;
}







