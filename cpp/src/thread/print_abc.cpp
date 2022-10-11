/***
 * @brief C++11 多线程入门练习
 * @details 三个线程顺序打印abc，共打印10次
 * @author qingl
 * @date 2022_09_13
 */
#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

void print_abc() {
    int count = 0;      //记录已打印的字符个数
    bool sign = true;   // 标记为 false 时打印完成，结束主进程
    std::mutex mtx;     // 上锁 count 和 sign

    auto print = [&](char c) {
        while (sign) {
            mtx.lock();
            if (count >= 10) {
                sign = false;
            }
            else if (count % 3 == c - 'a') {
                std::cout << c;
                count++;
            }
            mtx.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    };

    std::thread(print, 'a').detach();
    std::thread(print, 'b').detach();
    std::thread(print, 'c').detach();

    while (sign) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    std::cout << std::endl;
}

/***
 * @brief test
 * @author qingl
 * @date 2022_09_14
 */
#include <gtest/gtest.h>

TEST(thread, print_abc) {
    print_abc();
}
