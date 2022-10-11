#pragma once
// 排序算法 https://zh.wikipedia.org/wiki/%E6%8E%92%E5%BA%8F%E7%AE%97%E6%B3%95

/***
 * @brief 冒泡排序
 * @details order [_First, _Last) 稳定
 * 时间复杂度：n^2
 * https://www.runoob.com/w3cnote/bubble-sort.html
 * @author qingl
 * @date 2022_09_17
 */
template <class RanIt, class Pr>
void bubble_sort(const RanIt first, const RanIt last, Pr pred) {
    if (first >= last)
        return;
    for (auto i = first; i < last - 1; i++) {
        for (auto j = first; j < last - 1 - (i - first); j++) {
            if (!pred(*j, *(j + 1)))
                std::swap(*j, *(j + 1));
        }
    }
}

/***
 * @brief 选择排序
 * @details order [_First, _Last) 不稳定
 * 时间复杂度：n^2
 * https://www.runoob.com/w3cnote/selection-sort.html
 * @author qingl
 * @date 2022_09_25
 */
template <class RanIt, class Pr>
void selection_sort(const RanIt first, const RanIt last, Pr pred) {
    if (first >= last)
        return;
    for (auto i = first; i < last - 1; i++) {
        auto min = i;
        for (auto j = i + 1; j < last; j++)
            if (pred(*j, *min))
                min = j;
        std::swap(*i, *min);
    }
}

/***
 * @brief 插入排序
 * @details order [_First, _Last) 稳定
 * 时间复杂度：n^2
 * https://www.runoob.com/w3cnote/insertion-sort.html
 * @author qingl
 * @date 2022_09_25
 */
template <class RanIt, class Pr>
void insertion_sort(const RanIt first, const RanIt last, Pr pred) {
    if (first >= last)
        return;
    for (auto i = first + 1; i < last; i++) {
        auto key = *i;
        auto j = i;
        while (j > first && pred(key, *(j - 1))) {
            *j = *(j - 1);
            j--;
        }
        *j = key;
    }
}

/***
 * @brief 希尔排序
 * @details order [_First, _Last) 不稳定
 * 时间复杂度：nlog^2n
 * https://zh.wikipedia.org/wiki/%E5%B8%8C%E5%B0%94%E6%8E%92%E5%BA%8F
 * @author qingl
 * @date 2022_09_25
 */
template <class RanIt, class Pr>
void shell_sort(const RanIt first, const RanIt last, Pr pred) {
    if (first >= last)
        return;

    size_t h = 1;
    while (h < (last - first) / 3ull) {
        h = 3 * h + 1;
    }
    while (h >= 1) {
        for (auto i = first + h; i < last; i++) {
            for (auto j = i; j >= (first + h) && pred(*j, *(j - h)); j -= h) {
                std::swap(*j, *(j - h));
            }
        }
        h = h / 3;
    }
}

/***
 * @brief 归并排序
 * @details order [_First, _Last) 稳定
 * 时间复杂度：nlogn
 * 空间复杂度：k
 * https://www.runoob.com/w3cnote/merge-sort.html
 * @author qingl
 * @date 2022_09_25
 */
template <class RanIt, class Pr>
void merge_sort(const RanIt first, const RanIt last, Pr pred) {
    if (first >= last)
        return;

    size_t len = last - first;
    auto a = first._Ptr;
    auto b = new RanIt::value_type[len];
    for (size_t seg = 1; seg < len; seg += seg) {
        for (size_t start = 0; start < len; start += seg + seg) {
            size_t low = start, mid = std::min(start + seg, len),
                   high = std::min(start + seg + seg, len);
            size_t k = low;
            size_t start1 = low, end1 = mid;
            size_t start2 = mid, end2 = high;
            while (start1 < end1 && start2 < end2)
                b[k++] = pred(a[start1], a[start2]) ? a[start1++] : a[start2++];
            while (start1 < end1)
                b[k++] = a[start1++];
            while (start2 < end2)
                b[k++] = a[start2++];
        }
        std::swap(a, b);
    }
    if (a != first._Ptr) {
        for (size_t i = 0; i < len; i++)
            b[i] = a[i];
        b = a;
    }
    delete[] b;
}

/***
 * @brief 快速排序
 * @details order [_First, _Last) 不稳定
 * 时间复杂度：nlogn
 * 空间复杂度：n^2
 * https://www.runoob.com/w3cnote/quick-sort-2.html
 * 
 * TODO: pred
 * 
 * @author qingl
 * @date 2022_09_26
 */
template <class RanIt, class Pr>
void quick_sort(const RanIt first, const RanIt last, Pr pred) {
    if (first >= last)
        return;

    auto low = first, high = last - 1;
    typename RanIt::value_type pivot = *low;
    while (low < high) {
        while (low < high && *high >= pivot) {
            --high;
        }
        *low = *high;
        while (low < high && *low <= pivot) {
            ++low;
        }
        *high = *low;
    }
    *low = pivot;

    quick_sort(first, low, pred);
    quick_sort(low + 1, last, pred);
}

/***
 * @brief 堆排序
 * @details order [_First, _Last) 不稳定
 * 时间复杂度：nlogn
 * https://www.runoob.com/w3cnote/heap-sort.html
 * 
 * 使用大顶堆实现
 * 通常堆是通过一维数组来实现的。在数组起始位置为0的情形中
 * 父节点 i 的左子节点在位置 2i + 1
 * 父节点 i 的右子节点在位置 2i + 2
 * 子节点 i 的父节点在位置 (i - 1)/2
 * 
 * TODO: pred
 * 
 * @author qingl
 * @date 2022_09_26
 */
template <class RanIt, class Pr>
void heap_sort(const RanIt first, const RanIt last, Pr pred) {
    if (first >= last)
        return;

    // 最大堆调整（Max Heapify）：将堆的末端子节点作调整，使得子节点永远小于父节点
    auto max_heapify = [&pred](const RanIt first, const RanIt last) {
        // 建立父节点指标和子节点指标
        size_t dad = 0;
        size_t son = dad * 2 + 1;
        size_t end = last - first;
        while (son <= end) {   // 若子节点指标在范围内才做比较
            if (son + 1 <= end &&
                *(first + son) <
                    *(first + son + 1))   // 先比较两个子节点大小，选择最大的
                son++;
            if (*(first + dad) >
                *(first +
                  son))   // 如果父节点大于子节点代表调整完毕，直接跳出函数
                return;
            else {   // 否则交换父子内容再继续子节点和孙节点比较
                std::swap(*(first + dad), *(first + son));
                dad = son;
                son = dad * 2 + 1;
            }
        }
    };

    // 初始化，i从最后一个父节点开始调整
    int len = int(last - first); // TODO: size_t
    for (int i = len / 2 - 1; i >= 0; i--)
        max_heapify(first + i, first + (len - 1));
    // 先将第一个元素和已经排好的元素前一位做交换，再从新调整(刚调整的元素之前的元素)，直到排序完毕
    for (int i = len - 1; i > 0; i--) {
        std::swap(*first, *(first + i));
        max_heapify(first, first + (i - 1));
    }
}
