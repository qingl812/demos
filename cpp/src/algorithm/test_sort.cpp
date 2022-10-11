#include "sort.hpp"
#include <algorithm>
#include <gtest/gtest.h>
#include <vector>

#define test_sort(sort_func)                                                   \
    do {                                                                       \
        auto test_single_data = [&](std::vector<int> array) {                  \
            std::vector<int> expected;                                         \
            std::vector<int> actual;                                           \
                                                                               \
            actual = array;                                                    \
            sort_func(actual.begin(), actual.end(), std::less<>{});            \
            expected = array;                                                  \
            std::sort(expected.begin(), expected.end(), std::less<>{});        \
            EXPECT_EQ(actual, expected);                                       \
                                                                               \
            actual = array;                                                    \
            sort_func(actual.begin(), actual.end(), std::greater<>{});         \
            expected = array;                                                  \
            std::sort(expected.begin(), expected.end(), std::greater<>{});     \
            EXPECT_EQ(actual, expected);                                       \
        };                                                                     \
                                                                               \
        test_single_data({61, 17, 29, 22, 34, 60, 72, 21, 50, 1, 62});         \
        test_single_data({1, 2, 3, 4, 5, 6});                                  \
        test_single_data({6, 5, 4, 3, 2, 1});                                  \
        test_single_data({1, 2});                                              \
        test_single_data({2, 1});                                              \
        test_single_data({1});                                                 \
        test_single_data({});                                                  \
    } while (0)

TEST(algorithm_sort, all) {
    test_sort(bubble_sort);
    test_sort(selection_sort);
    test_sort(insertion_sort);
    test_sort(shell_sort);
    test_sort(merge_sort);
    // test_sort(quick_sort);
    // test_sort(heap_sort);
}
