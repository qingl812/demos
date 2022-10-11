/**
 * @brief 内联汇编
 *
 * @details
 * 汇编语言以两种形式出现：英特尔风格和 AT&T 风格。
 * GNU C 编译器，即 GCC 使用 AT&T 语法，这就是我们将使用的。
 *
 * 1. 寄存器命名：
 *      寄存器名称以 % 为前缀， 因此寄存器为 %eax、%cl 等，
 *      而不仅仅是 eax、cl。
 * 2. 操作数的顺序：
 *      与 Intel 约定（第一个操作数是目标）不同，
 *      操作数的顺序是源在前，目标在后。
 *      例如，英特尔语法“mov eax, edx”在 AT&T 汇编中看起来像“mov %edx, %eax”。
 * 3. 操作数大小：
 *      在 AT&T 语法中，内存操作数的大小由操作码名称的最后一个字符确定。
 *      后缀是 b 表示（8 位）字节，w 表示（16 位）字，l 表示（32 位）长。
 *      例如，上述指令的正确语法应该是“movl %edx, %eax”。
 * 4. 立即数操作数：
 *      立即数操作数用 $ 前缀标记，如“addl $5, %eax”，
 *      表示将立即数长值 5 添加到寄存器 %eax）。
 * 5. 内存操作数：
 *      缺少操作数前缀表示它是内存地址；
 *      因此 "movl $bar, %ebx" 将变量 bar 的地址放入寄存器 %ebx，
 *      而 "movl bar, %ebx" 将变量 bar 的内容放入寄存器 %ebx。
 * 6. 索引：
 *      索引或间接通过将索引寄存器或间接存储单元地址括在括号中来完成。
 *      例如，“movl 8(%ebp), %eax”
 *      （将 %ebp 指向的单元格偏移 8 处的内容移动到寄存器 %eax 中）。
 *
 * https://www.codeproject.com/Articles/15971/Using-Inline-Assembly-in-C-C
 *
 * @note
 * can't run example_6 and example_7
 */

#include <gtest/gtest.h>
#include <stdio.h>

TEST(inline_assembly, example_0) {
    /* Add 10 and 20 and store result into register %eax */
    __asm__(
        "movl $10, %eax;"
        "movl $20, %ebx;"
        "addl %ebx, %eax;");

    /* Subtract 20 from 10 and store result into register %eax */
    __asm__(
        "movl $10, %eax;"
        "movl $20, %ebx;"
        "subl %ebx, %eax;");

    /* Multiply 10 and 20 and store result into register %eax */
    __asm__(
        "movl $10, %eax;"
        "movl $20, %ebx;"
        "imull %ebx, %eax;");
}

/***
 * 在本例中，变量“val”保存在寄存器中，
 * 寄存器 eax 中的值被复制到该寄存器中，
 * “val”的值从该寄存器更新到内存中。
 * 当指定“r”约束时，
 * gcc 可以将变量保存在任何可用的通用寄存器中。
 * 我们还可以通过使用特定的寄存器约束直接指定寄存器名称。
 * 寄存器约束如下：
 * +---+--------------------+
 * | r |    Register(s)     |
 * +---+--------------------+
 * | a |   %eax, %ax, %al   |
 * | b |   %ebx, %bx, %bl   |
 * | c |   %ecx, %cx, %cl   |
 * | d |   %edx, %dx, %dl   |
 * | S |   %esi, %si        |
 * | D |   %edi, %di        |
 * +---+--------------------+
 */
TEST(inline_assembly, example_1) {
    // third in example_0
    /* Multiply 10 and 20 and store result into register %eax */
    __asm__(
        "movl $10, %eax;"
        "movl $20, %ebx;"
        "imull %ebx, %eax;");

    int val;
    asm("movl %%eax, %0;" : "=r"(val));
    printf("example 1: %d\n", val);
}

/***
 * 在本示例中，“val”是输出操作数，由 %0 引用，
 * “no”是输入操作数，由 %1 引用。
 * “r”是对操作数的约束，它告诉 GCC 使用任何寄存器来存储操作数。
 * 输出操作数约束应该有一个约束修饰符“=”来指定只写模式下的输出操作数。
 * 寄存器名称前有两个 % 前缀，这有助于 GCC 区分操作数和寄存器。
 * 操作数有一个 % 作为前缀。
 * 第三个冒号后被破坏的寄存器 %ebx 通知 GCC %ebx 的值将在 "asm" 中修改，
 * 因此 GCC 不会使用该寄存器来存储任何其他值。
 */
TEST(inline_assembly, example_2) {
    int val, no = 100;
    asm("movl %1, %%ebx;"
        "movl %%ebx, %0;"
        : "=r"(val) /* output */
        : "r"(no)   /* input */
        : "%ebx"    /* clobbered register */
    );
    printf("example 2: %d %d\n", no, val);
}

/***
 * 这里的“add”是寄存器 eax 引用的输出操作数。
 * arg1 和 arg2 分别是寄存器 eax 和 ebx 引用的输入操作数。
 * 让我们看一个使用扩展内联汇编语句的完整示例。
 * 它对整数操作数执行简单的算术运算并显示结果（在下载中作为算术.c
 * 提供）。
 */
TEST(inline_assembly, example_3) {
    int arg1 = 1, arg2 = 2, add;
    __asm__("addl %%ebx, %%eax;" : "=a"(add) : "a"(arg1), "b"(arg2));
    printf("example 3: %d %d %d\n", arg1, arg2, add);
}

TEST(inline_assembly, example_4) {
    int arg1, arg2, add, sub, mul, quo, rem;

    arg1 = 4;
    arg2 = 2;

    /* Perform Addition, Subtraction, Multiplication & Division */
    __asm__("addl %%ebx, %%eax;" : "=a"(add) : "a"(arg1), "b"(arg2));
    __asm__("subl %%ebx, %%eax;" : "=a"(sub) : "a"(arg1), "b"(arg2));
    __asm__("imull %%ebx, %%eax;" : "=a"(mul) : "a"(arg1), "b"(arg2));

    __asm__(
        "movl $0x0, %%edx;"
        "movl %2, %%eax;"
        "movl %3, %%ebx;"
        "idivl %%ebx;"
        : "=a"(quo),   // 整除结果
          "=d"(rem)    // 取余结果
        : "g"(arg1), "g"(arg2));

    printf("example 4:\n");
    printf("%d + %d = %d\n", arg1, arg2, add);
    printf("%d - %d = %d\n", arg1, arg2, sub);
    printf("%d * %d = %d\n", arg1, arg2, mul);
    printf("%d / %d = %d\n", arg1, arg2, quo);
    printf("%d %% %d = %d\n", arg1, arg2, rem);
}

/***
 * 如果我们的汇编语句必须在我们放置的地方执行，
 * （即不能作为优化移出循环），
 * 请将关键字“volatile”或
 * “__volatile__”放在
 * “asm”或“__asm__”之后和（）之前.
 */
// 请参考以下示例，
// 该示例使用众所周知的欧几里得算法（被誉为第一算法）计算最大公约数。
int gcd(int a, int b) {
    int result;
    /* Compute Greatest Common Divisor using Euclid's Algorithm */
    __asm__ __volatile__(
        "movl %1, %%eax;"
        "movl %2, %%ebx;"
        "CONTD: cmpl $0, %%ebx;"
        "je DONE;"
        "xorl %%edx, %%edx;"
        "idivl %%ebx;"
        "movl %%ebx, %%eax;"
        "movl %%edx, %%ebx;"
        "jmp CONTD;"
        "DONE: movl %%eax, %0;"
        : "=g"(result)
        : "g"(a), "g"(b));

    return result;
}

TEST(inline_assembly, example_5) {
    int first = 6, second = 9;
    printf("example 5:\n");
    printf("GCD of %d & %d is %d\n", first, second, gcd(first, second));
}

// can't run example_6 and example_7
#define example_6_and_7 0
#if example_6_and_7
// 下面是更多使用 FPU（浮点单元）指令集的示例。
// 执行简单浮点运算的示例程序：
TEST(inline_assembly, example_6) {
    float arg1, arg2, add, sub, mul, div;

    printf("Enter two numbers : ");
    scanf("%f%f", &arg1, &arg2);

    // Perform floating point Addition, Subtraction, Multiplication & Division
    __asm__(
        "fld %1;"
        "fld %2;"
        "fadd;"
        "fstp %0;"
        : "=g"(add)
        : "g"(arg1), "g"(arg2));

    __asm__(
        "fld %2;"
        "fld %1;"
        "fsub;"
        "fstp %0;"
        : "=g"(sub)
        : "g"(arg1), "g"(arg2));

    __asm__(
        "fld %1;"
        "fld %2;"
        "fmul;"
        "fstp %0;"
        : "=g"(mul)
        : "g"(arg1), "g"(arg2));

    __asm__(
        "fld %2;"
        "fld %1;"
        "fdiv;"
        "fstp %0;"
        : "=g"(div)
        : "g"(arg1), "g"(arg2));

    printf("%f + %f = %f\n", arg1, arg2, add);
    printf("%f - %f = %f\n", arg1, arg2, sub);
    printf("%f * %f = %f\n", arg1, arg2, mul);
    printf("%f / %f = %f\n", arg1, arg2, div);
}

// example 7
// 计算三角函数（如 sin 和 cos）的示例程序：
float sinx(float degree) {
    float result, two_right_angles = 180.0f;
    /* Convert angle from degrees to radians and then calculate sin value */
    __asm__ __volatile__(
        "fld %1;"
        "fld %2;"
        "fldpi;"
        "fmul;"
        "fdiv;"
        "fsin;"
        "fstp %0;"
        : "=g"(result)
        : "g"(two_right_angles), "g"(degree));
    return result;
}

float cosx(float degree) {
    float result, two_right_angles = 180.0f, radians;
    /* Convert angle from degrees to radians and then calculate cos value */
    __asm__ __volatile__(
        "fld %1;"
        "fld %2;"
        "fldpi;"
        "fmul;"
        "fdiv;"
        "fstp %0;"
        : "=g"(radians)
        : "g"(two_right_angles), "g"(degree));
    __asm__ __volatile__(
        "fld %1;"
        "fcos;"
        "fstp %0;"
        : "=g"(result)
        : "g"(radians));
    return result;
}

float square_root(float val) {
    float result;
    __asm__ __volatile__(
        "fld %1;"
        "fsqrt;"
        "fstp %0;"
        : "=g"(result)
        : "g"(val));
    return result;
}

TEST(inline_assembly, example_7) {
    float theta;
    printf("Enter theta in degrees : ");
    scanf("%f", &theta);

    printf("sinx(%f) = %f\n", theta, sinx(theta));
    printf("cosx(%f) = %f\n", theta, cosx(theta));
    printf("square_root(%f) = %f\n", theta, square_root(theta));
}
#endif
