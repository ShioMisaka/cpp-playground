#pragma once 

#include <cstddef>
#include <stdexcept>

#include "algorithm/algobase.h"

namespace mystl
{

template <typename Tp, std::size_t N>
struct array
{
public:
    using value_type      = Tp;
    using size_type       = size_t;
    using reference       = value_type &;
    using const_reference = const value_type &;
    using pointer         = value_type *;
    using const_pointer   = const value_type *;

    using iterator        = pointer;
    using const_iterator  = const_pointer;

    value_type elems_[N];

    // ========== 容量 ==========
    constexpr size_type size() const noexcept { return N; };
    constexpr bool empty() const noexcept { return N == 0; };

    reference operator[](size_type i) { return elems_[i]; }
    const_reference operator[](size_type i) const { return elems_[i]; }

    reference at(size_type pos)
    {
        if (pos < N) return elems_[pos];
        else throw std::out_of_range("mystl::array out of range.");
    }

    const_reference at(size_type pos) const
    {
        if (pos < N) return elems_[pos];
        else throw std::out_of_range("mystl::array out of range.");
    }

    iterator begin() noexcept { return elems_; }
    const_iterator begin() const noexcept { return elems_; }
    iterator end() noexcept { return elems_ + N; }
    const_iterator end() const noexcept { return elems_ + N; }

    void fill(const value_type& value) 
    {
        for (size_type i = 0; i < N; ++i) elems_[i] = value;
    }

    void swap(array& other) 
    {
        if (other.size() != N) return;
        for (size_type i = 0; i < N; ++i)
            mystl::swap(elems_[i], other.elems_[i]);
    } 
};
}