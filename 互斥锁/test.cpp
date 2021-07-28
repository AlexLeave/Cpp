#include "lock.hpp"
#include "thread.hpp"
#include <iostream>
#include <unistd.h>


class CLock *lock = new CLock();

int x = 10;

void *child_thread(void *arg)
{
    lock->lock(block);
    sprintf(a,);
    std::cout << "使用中\n";
    x += 1;
    std::cout << x << std::endl;
    x = x*10;
    std::cout << x << std::endl;
    std::cout << "用完了\n";
    sleep(1);
    lock->unlock();

    return 0;

}


int main(int argc, char const *argv[])
{
    class CThread *thread_f = new CThread(child_thread, NULL);
    class CThread *thread_s = new CThread(child_thread, NULL);

    sleep(5);
    delete thread_f;
    delete thread_s;

    return 0;
}


