#include "share_mem.hpp"
#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{
    class CShmem *mem = new CShmem();


    // mem->erase_shmem();
    printf("%s", (char *)mem->Ptr());
    mem->Depart();
    mem->Destory();

    return 0;
}