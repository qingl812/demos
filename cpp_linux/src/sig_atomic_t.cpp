/**
 * @brief 类型 sig_atomic_t
 * @details
 * sig_atomic_t这个类型是定义在signal.h文件中。
 * 当把变量声明为该类型会保证该变量在使用或赋值时，
 * 无论是在32位还是64位的机器上都能保证操作是原子的，
 * 它会根据机器的类型自动适应。
 *
 * 在处理信号(signal)的时候，
 * 有时对于一些变量的访问希望不会被中断，
 * 无论是硬件中断还是软件中断，
 * 这就要求访问或改变这些变量需要在计算机的一条指令内完成。
 * 通常情况下，int类型的变量通常是原子访问的，
 * 也可以认为 sig_atomic_t就是int类型的数据，
 * 因为对这些变量要求一条指令完成，所以sig_atomic_t不可能是结构体，
 * 只会是数字类型。
 * 在linux里这样定义：
 * @ref typedef int __sig_atomic_t;
 * 另外gnu c的文档也说比int短的类型通常也是具有原子性的，
 * 例如short类型。同时，指针（地址）类型也一定是原子性的。
 * 该类型在所有gnu c库支持的系统和支持posix的系统中都有
 *
 * @details
 * https://en.cppreference.com/w/c/program/sig_atomic_t
 * https://baike.baidu.com/item/sig_atomic_t/10829369
 */
#include <gtest/gtest.h>
#include <signal.h>
#include <stdio.h>

volatile sig_atomic_t gSignalStatus = 0;

void signal_handler(int signal) {
    gSignalStatus = signal;
}

TEST(sig_atomic_t, example) {
    /* Install a signal handler. */
    signal(SIGINT, signal_handler);

    printf("SignalValue:   %d\n", gSignalStatus);
    printf("Sending signal %d\n", SIGINT);
    raise(SIGINT);
    printf("SignalValue:   %d\n", gSignalStatus);
}
