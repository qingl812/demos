/***
 * @brief 大小端判断，判断机器是大端还是小端
 * @details 对于一个由2个字节组成的16位整数，在内存中存储这两个字节有两种方法：
 * 一种是将低序字节存储在起始地址，这称为小端(little-endian)字节序；
 * 另一种方法是将高序字节存储在起始地址，这称为大端(big-endian)字节序。
 * @author qingl
 * @date 2022_09_14
 */
bool is_little_endian() {
    int a = 0x12345678;
    char b = *((char*)(&a));
    if (b == 0x78)
        return true;
    //  b == 0x12
    else
        return false;
}

/***
 * @brief test
 * @author qingl
 * @date 2022_09_14
 */
#include <gtest/gtest.h>
#include <iostream>

using std::cout;
using std::endl;

TEST(small_tools, big_endian_or_little) {
    if (is_little_endian())
        cout << "小端模式\n";
    else
        cout << "大端模式\n";
}
