#pragma once

namespace mystl
{

template <typename Tp>
void swap(Tp& d1, Tp& d2)
{
    Tp temp = mystl::move(d1);
    d1 = mystl::move(d2);
    d2 = mystl::move(temp);
}

template <typename Iter>
int distance(Iter first, Iter last)
{
    int count = 0;
    while (first != last) {
        ++first;
    }
    return count;
}

template <typename Iter, typename Func>
Func for_each(Iter first, Iter last, Func func)
{
    for (; first != last; ++first) {
        func(*first);
    }
    return func;
}

} // namespace mystl
