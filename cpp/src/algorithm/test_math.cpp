#include "math.hpp"
#include <gtest/gtest.h>
#include <iostream>

TEST(algorithm_math, gcd) {
    EXPECT_EQ(gcd(27, 36), 9);
}

TEST(algorithm_math, lcm) {
    EXPECT_EQ(lcm(27, 36), 108);
}

TEST(algorithm_math, get_pi) {
    double pi = get_pi();
    // #include<limits>
    std::cout.precision(std::numeric_limits<double>::max_digits10);
    // cout.precision(17);
    std::cout << "pi: " << pi << std::endl;
}
