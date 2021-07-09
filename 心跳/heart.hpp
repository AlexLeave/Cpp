/**
 * @file heart.hpp
 * @author csu-lf (csu.lifeng@qq.com)
 * @brief 封装一个用于检测心跳的类，用起来真的很方便呢，使用计数器思路设计检测心跳子线程
 * @version 1.1
 * @date 2021-07-06
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#ifndef _MY_HEART_H
#define _MY_HEART_H 1

#include <iostream>
#include <stdio.h>
#include <thread>
#include <unistd.h>

class CHeart{
        private:
        bool alive = true;
        bool heart = true;
        int T = 5; // 5s 一个周期
        std::thread *beat_thread;
        int id;

        /**
         * @brief 检查心跳子线程，把时间轴按照周期 T 间隔，每个区间有心跳心脏就不会死
         * 
         */
        static void check_beats(class CHeart *obj)
        {
            while(true)
            {
                // std::cout << time(NULL) << std::endl;
                sleep(obj->get_T()); // 每 T秒 检查是否在心跳
                // std::cout << time(NULL) << std::endl;
                if(obj->get_heart() && obj->IsAlive()) 
                { 
                    obj->heart_down(); obj->set_alive(true); 
                    printf("heart keeping...\n"); 
                } // 在心跳
                else 
                { 
                    obj->set_alive(false); 
                    printf("no heart beat, died\n"); break; 
                } // 没有心跳了
            }
        }


        /**
         * @brief 升级版检查心跳子线程，精度为 一秒，每次心跳完后必须在 T秒内 再次心跳心脏才不会死
         * 
         * @param obj 
         * @param others 
         */
        static void check_beats_exact(class CHeart *obj, void * others = NULL)
        {
            int cnt = 0; // 精确检测心跳线程函数的计数器
            while(cnt < obj->get_T())
            {
                sleep(1); // 每 1秒 检查是否在心跳
                if(obj->get_heart() && obj->IsAlive()) 
                {  // 这一秒在心跳，且心脏没有被杀死
                    cnt = 0; obj->heart_down(); // 计数器复位
                    obj->set_alive(true); 
                    printf("heart beat keeping...\n"); 
                }
                else if(!obj->get_heart() && obj->IsAlive()) 
                { // 这一秒没有心跳，且心脏没有被杀死
                    cnt++;
                    printf("heart no beat in 1 sec\n");
                }
                else // 心脏被杀死
                {
                    printf("heart has been killed\n");
                    break;
                }
            }
            // 出循环代表心脏已死，或心跳已经无法维持心脏
            if(obj->IsAlive()) printf("heart weak, heart is dying\n");


            obj->set_alive(false);

            printf("heart died\n");
        }



        // 给检测心跳线程操作私有变量的私有函数
        bool get_heart() { return heart; }
        bool heart_down() { heart = false; return !heart; }
        bool heart_up() { heart = true; return heart; }
        bool set_alive(bool Alive) { alive = Alive; return alive; }

        public:
        CHeart(int ID = 0)
        {
            id = ID;
        }

        /**
         * @brief 获取创建此类变量时的id
         * 
         * @return int id号
         */
        int get_id() { return id; }

        /**
         * @brief 设置这颗 heart 的ID，用户以此区分它们
         * 
         * @param ID 设置成这个id
         * @return int 设置成功返回输入的ID
         */
        int set_id(int ID) {id = ID; return id; }

        /**
         * @brief 返回监测心跳函数子线程指针
         * 
         * @return std::thread* 
         */
        std::thread *beat_thread(){ return beat_thread; }

        /**
         * @brief 返回是否需要判断的物体存活
         * 
         * @return true 存活
         * @return false 挂掉
         */
        bool IsAlive() { return alive; }

        /**
         * @brief 保持心跳，调用一次相当于心跳一次
         * 
         * @return true 保持成功
         * @return false 保持失败
         */
        bool beat() { heart = true; return heart; }

        /**
         * @brief 查看物体心跳间隔时间
         * 
         * @return int 物体心跳间隔时间
         */
        int get_T() { return T; } 

        /**
         * @brief 设置物体心跳间隔时间
         * 
         * @param t 需要把心跳间隔时间设置成t秒
         * @return int 变化后的心跳间隔时间T
         */
        int set_T(int t) { T = t; return T;}


        /**
         * @brief 初始化，创建子线程检查每 t秒 的心跳，第一次检查是 t秒 后
         * 
         * @param t 需要把心跳间隔时间设置成t秒 
         * @param others 其它你想要传递给心跳监测函数的东西都可以放在这里，然后自己添加内容
         * 
         * @return std::thread* 返回心跳子线程
         */
        std::thread * init(int t = 5, void * others = NULL)
        {
            set_T(t);
            if(alive) beat_thread = new std::thread(check_beats_exact, this, others);

            return beat_thread;
        }

        /**
         * @brief 杀死这个 heart
         * 
         * @return true 杀死成功
         * @return false 杀死失败
         */
        bool heart_kill()
        {
            alive = false;
            return !heart;
        }
        


    };


#endif
