#pragma once 

#include <functional>

#include "../utility.h"
#include "algobase.h"

namespace mystl
{
// 冒泡排序
template <typename Iter, typename Compare = std::less<>>
void bubble_sort(Iter first, Iter last, Compare comp = {})
{
    for (auto it = first; it != last; ++it) {
        bool swapped = false;
        for (auto jt = first; jt != last - 1 - (it - first); ++jt) {
            if (comp(*(jt + 1), *jt)) {
                swap(*jt, *(jt + 1));
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

// 选择排序
template <typename Iter, typename Compare = std::less<>>
void select_sort(Iter first, Iter last, Compare comp = {})
{
    for (auto it = first; it != last; ++it) {
        Iter mini_it = it;
        for (auto jt = it + 1; jt != last; ++jt) {
            if (comp(*jt, *mini_it)) {
                mini_it = jt;
            }
        }
        swap(*it, *mini_it);
    }
}

// ---- 快速排序 ----
template <typename Iter, typename Compare>
Iter partition(Iter first, Iter last, Compare comp) {
    while (first < last) {
        while (first < last && comp(*first, *last)) --last;
        mystl::swap(*first, *last);
        while (first < last && comp(*first, *last)) ++first;
        mystl::swap(*first, *last);
    }
    return first;
}

template <typename Iter, typename Compare = std::less<>>
void quick_sort(Iter first, Iter last, Compare comp = {})
{
    if (first < last - 1) {
        auto mid = mystl::partition(first, last - 1, comp);
        quick_sort(first, mid + 1, comp);
        quick_sort(mid + 1, last, comp);
    }
}

} // namespace mystl
