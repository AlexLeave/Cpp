#include "sem.hpp"
#include <iostream>
#include <unistd.h>

using namespace std;

int main(int argc, char const *argv[])
{
    class CSem *sem = new CSem(0x1001);

    sem->lock_sem();
    cout << "使用中" << endl;

    sleep(5);

    sem->unlock_sem();
    cout << "用完了\n";

    // sem->delete_sem();
    delete sem;


    return 0;
}





