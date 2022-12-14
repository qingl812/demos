#include <functional>
#include <iostream>

using std::cout;
using std::endl;

/***
 * @brief 空类对象的大小
 * @author qingl
 * @date 2022_09_14
 */
void size_of_empty_class_object() {
    class EmptyClass
    {
    };

    EmptyClass e;
    cout << "size_of_empty_class_object: " << sizeof(e) << endl;
    cout << endl;
}

/***
 * @brief 类成员构造顺序
 * @details 在没有初始化列表时，按声明顺序初始化成员变量
 * 若有初始化列表，先按声明顺序初始化列表中变量，而后在初始化其他（全都按照声明顺序初始化）
 * @author qingl
 * @date 2022_09_14
 */
void class_member_construction_order() {
    class ClassA
    {
    public:
        ClassA() {
            cout << "Constructor ClassA.\n";
        }
    };

    class ClassB
    {
    public:
        ClassB() {
            cout << "Constructor ClassB.\n";
        }
    };

    class ClassC
    {
    public:
        ClassC() {
            cout << "Constructor ClassC.\n";
        }
    };

    class ClassTa
    {
    public:
        ClassTa() {
            cout << "Constructor ClassTa.\n";
        }
        ClassTa(ClassC c, ClassB b) : m_c(c), m_b(b) {
            cout << "Constructor ClassTa.\n";
        }

        ClassA m_a;
        ClassB m_b;
        ClassC m_c;
    };

    cout << "class_member_construction_order: \n";
    ClassTa ta;
    cout << endl;

    ClassB b;
    ClassC c;
    ClassTa ta2(c, b);
    cout << endl;
}

/***
 * @brief 类中的函数
 * @details 其实类中的函数和类外面的函数本质是一样的，只不过多了一个隐含的参数
 * 指向对象的指针，即 this 指针。
 * 
 * C++ 中没有提供获取指向类的成员函数的指针的方法，
 * 但是可以通过虚函数表获取虚函数指针，详见 “虚函数用法”
 * 
 * 也许可以通过反汇编等手段获取成员函数指针，但这个方法没有任何实际作用，
 * 详见 ./get_member_function_pointer.cpp
 * 
 * @author qingl
 * @date 2022_09_14
 */
void function_in_class() {
    class ClassA
    {
    public:
        ClassA(int data = 10) : m_data(data){};

        // 打印 data + target 的值
        void print_sum(int target) {
            cout << m_data + target << endl;
        }

        // 打印 data + target 的值
        void print_avg(int target) {
            cout << (m_data + target) / 2 << endl;
        }

    private:
        int m_data;
    };

    cout << "function_in_class: \n";
    ClassA a;
    a.print_sum(5);

    // &ClassA::print_sum 是成员指针类型，它类似于函数指针，是一种新类型
    auto func_p = &ClassA::print_sum;
    (a.*func_p)(6);
    func_p = &ClassA::print_avg;
    (a.*func_p)(6);

    // 使用 std::bind 调用类成员函数, std::placeholders::_1为占位符
    auto func = std::bind(&ClassA::print_sum, &a, std::placeholders::_1);
    func(7);
    cout << endl;
}

/***
 * @brief 虚函数用法
 * @author qingl
 * @date 2022_09_14
 */
void virtual_function() {
    class Parent
    {
    public:
        // 或者可以这样定义，但此时不能定义 Parent 对象（Parent p;）
        // virtual void print() = 0;
        virtual void print() {
            cout << "The print function of the parent class.\n";
        }

        void do_something() {
            print();
        }

        virtual void a() {
            cout << "function a\n";
        };

        virtual void b() {
            cout << "function b\n";
        };

        virtual void c() {
            cout << "function c\n";
        };

        virtual void print_sum(size_t n) {
            cout << (n + m_data) << endl;
        };

        size_t m_data = 10;
    };

    class Son : public Parent
    {
    public:
        virtual void print() {
            cout << "The print function of the son class.\n";
        }
    };

    cout << "virtual_function: \n";
    // parent 和 son 都是 Parent 的对象，但是能表现出不同的行为，这就是多态
    Parent* parent = new Parent;
    parent->do_something();

    Parent* son = new Son;
    son->do_something();

    // 带有虚函数的类都有虚函数表，虚函数表就是一个存储着函数指针的数组
    // 所以带虚函数的类都要有 8 个字节用来存储指向虚函数表的指针
    cout << "Parent size: " << sizeof(*parent) << endl;

    // 通过这个虚函数表，就可以获取类中的函数指针，而不是 &ClassA::print_sum 这种成员指针
    // 当然这只能获取虚函数的指针，普通函数的指针还是没法获取
    using func_t = void (*)();   // 函数指针类型
    using virtual_list_t = func_t*;   // 虚函数表类型(存储函数指针的数组)
    using virtual_list_pt = virtual_list_t*;   // 指向虚函数表的指针类型

    // 虚表存储在头部，之后才是成员变量
    auto list_ptr = (virtual_list_pt)parent;
    auto list = *list_ptr;
    // (*(list + 0))(); // 效果等同于 list[0]();
    list[0]();
    list[1]();
    list[2]();
    list[3]();
    // 如果类 Parent 中的函数指针类型不同，使用强制类型转换转化一下即可
    using func_sum_t = void (*)(Parent*, size_t);
    auto func_sum = (func_sum_t)list[4];
    func_sum(parent, 1);
    cout << endl;
}

/***
 * @brief 虚函数和构造函数
 * @author qingl
 * @date 2022_09_14
 */
void virtual_and_constructor() {
    class Parent
    {
    public:
        Parent() {
            print();
        }

        virtual void print() {
            cout << "The print function of the parent class.\n";
        }
        // error: 局部类的引用成员函数或虚拟成员函数必须进行定义
        // virtual void print() = 0;
    };

    class Son : public Parent
    {
    public:
        Son() {
            print();
        }

        virtual void print() {
            cout << "The print function of the son class.\n";
        }
    };

    cout << "virtual_and_constructor: \n";
    Son son;
    cout << endl;
}

/***
 * @brief test
 * @author qingl
 * @date 2022_09_14
 */
#include <gtest/gtest.h>

TEST(class, size_of_empty_class_object) {
    size_of_empty_class_object();
}

TEST(class, class_member_construction_order) {
    class_member_construction_order();
}

TEST(class, function_in_class) {
    function_in_class();
}

TEST(class, virtual_function) {
    virtual_function();
}

TEST(class, virtual_and_constructor) {
    virtual_and_constructor();
}
