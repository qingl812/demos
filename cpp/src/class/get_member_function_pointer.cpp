#include <iostream>

using std::cout;
using std::endl;

/***
 * @brief 获取类成员函数指针
 * @details 我们可以获得类中静态成员函数指针，把这个值加上一个 offset，
 * 就可以获得成员函数的指针，但是这个 offset 只能通过反汇编获取，
 * 且每当修改代码或切换机器时，offset 都可能会发生变化
 * @author qingl
 * @date 2022_09_14
 */
class ClassA
{
public:
    ClassA(int data) : m_data(data){};

    void funcB(int n) {
        cout << "member function: " << m_data + n << endl;
    }

    static void funcA() {
        cout << "static function\n";
    }

private:
    int m_data;
};

/***
 * @brief test
 * @author qingl
 * @date 2022_09_14
 */
#include <gtest/gtest.h>

// #define RUN_TEST
#ifdef RUN_TEST
TEST(class, get_member_function_pointer) {
    size_t offset = 15;
    // 指针实际就是一个 size_t 类型的值，占8个字节
    auto func_a = (size_t)ClassA::funcA;
    auto func_b = func_a + offset;

    ClassA ca(10);
    ClassA cb(20);
    // 通过反汇编查看并计算 funcA 和 funcB 两个函数指针的差值，赋值到 offset 变量中
    // 这个值是不确定的只能通过反汇编获得
    ClassA::funcA();
    ca.funcB(1);
    cb.funcB(2);

    cout << endl;
    ((void (*)())func_a)();
    ((void (*)(ClassA*, int))func_b)(&ca, 3);
    ((void (*)(ClassA*, int))func_b)(&cb, 4);
}
#endif
