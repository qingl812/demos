/**
 * @brief 设置一个函数来处理信号，即带有 sig 参数的信号处理程序。
 * @ref void (*signal(int sig, void (*func)(int)))(int)
 *
 * @param sig
 * 在信号处理程序中作为变量使用的信号码。下面是一些重要的标准信号常量：
 * SIGABRT	(Signal Abort)
 *              程序异常终止。
 * SIGFPE	(Signal Floating-Point Exception)
 *              算术运算出错，如除数为 0 或溢出（不一定是浮点运算）。
 * SIGILL	(Signal Illegal Instruction)
 *              非法函数映象，如非法指令，
 *              通常是由于代码中的某个变体或者尝试执行数据导致的。
 * SIGINT	(Signal Interrupt)
 *              中断信号，如 ctrl-C，通常由用户生成。
 * SIGSEGV	(Signal Segmentation Violation)
 *              非法访问存储器，如访问不存在的内存单元。
 * SIGTERM	(Signal Terminate)
 *              发送给本程序的终止请求信号。
 * @param func
 * 一个指向函数的指针。它可以是一个由程序定义的函数，也可以是下面预定义函数之一
 * SIG_DFL	默认的信号处理程序。
 * SIG_IGN	忽视信号。
 *
 * @return
 * 该函数返回信号处理程序之前的值，当发生错误时返回 SIG_ERR。
 *
 * @details
 * @ref raise() 函数发送一个信号
 * https://www.runoob.com/cprogramming/c-function-signal.html
 */
#include <gtest/gtest.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void sighandler(int signum) {
    printf("catch signal %d, jump out...\n", signum);
    exit(1);
}

int main() {
    signal(SIGINT, sighandler);

    while (1) {
        printf("sleep for a second...\n");
        sleep(1);
    }
}
