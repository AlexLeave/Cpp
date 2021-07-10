/**
 * @file test.cpp
 * @author csu-lf (csu.lifeng@qq.com)
 * @brief 测试heart.hpp
 * @version 0.1
 * @date 2021-07-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */



#include "heart.hpp"



void test_init()
{
    class CHeart heart = CHeart();

    heart.Init(10);

    sleep(20);
}

void test_beat()
{
    class CHeart heart = CHeart();

    heart.Init(10);

    for (int i = 0; i < 10; i++)
    {
        heart.Beat();
        sleep(5);
    }
}


int main(int argc, char const *argv[])
{
    test_beat();

    printf("in main\n");
    sleep(10);

    return 0;
}

