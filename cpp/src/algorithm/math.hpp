#pragma once
/***
 * @brief 最大公约数
 * @details 辗转相除法
 * @link https://zh.m.wikipedia.org/zh/%E6%9C%80%E5%A4%A7%E5%85%AC%E5%9B%A0%E6%95%B8
 * @author qingl
 * @date 2022_09_16
 */
template <typename T>
T gcd(T a, T b) {
    if (b)
        while ((a %= b) && (b %= a))
            ;
    return a + b;
}

/***
 * @brief 最小公倍数
 * @author qingl
 * @date 2022_09_16
 */
template <typename T>
T lcm(T a, T b) {
    return a * b / gcd(a, b);
}

/***
 * @brief 圆周率
 * @details 受到double精度的限制，可求位数太少故直接返回挺好，哈哈哈
 * @link https://www.wikiwand.com/zh/%E9%AB%98%E6%96%AF-%E5%8B%92%E8%AE%A9%E5%BE%B7%E7%AE%97%E6%B3%95
 * @author qingl
 * @date 2022_09_16
 */
double get_pi() {
    constexpr double pi = 3.14159265358979;
    return pi;
}
