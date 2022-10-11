/**
 * @brief 关键字 volatile
 * @details
 * volatile 指出 i 是随时可能发生变化的，
 * 每次使用它的时候必须从 i 的地址中读取，
 * 因而编译器生成的汇编代码会重新从 i 的地址读取数据放在 b 中。
 * 而优化做法是，
 * 由于编译器发现两次从 i 读数据的代码之间的代码没有对 i 进行过操作，
 * 它会自动把上次读的数据放在 b 中。
 * 而不是重新从 i 里面读。
 * 这样一来， 如果 i 是一个寄存器变量或者表示一个端口数据就容易出错，
 * 所以说 volatile 可以保证对特殊地址的稳定访问。
 *
 * @details
 * https://www.runoob.com/w3cnote/c-volatile-keyword.html
 * https://stackoverflow.com/questions/71716095/how-to-change-the-value-of-a-variable-without-the-compiler-knowing/71716449#71716449
 *
 * @note
 * gcc other/volatile.c -o out/a.out -O0 -lpthread  && out/a.out
 * gcc other/volatile.c -o out/a.out -O3 -lpthread  && out/a.out
 */

#include <pthread.h>
#include <stdio.h>

int testVar;
volatile int waitVar;

void* otherThread(void* dummy) {
    while (waitVar != 2) { /* Wait */
    }
    testVar = 123;
    waitVar = 3;
    return NULL;
}

int main() {
    pthread_t pt;
    waitVar = 1;
    pthread_create(&pt, 0, otherThread, NULL);
    testVar = 10;
    waitVar = 2;
    while (waitVar != 3) { /* Wait */
    }
    printf("%d\n", testVar - 10);
    return 0;
}
