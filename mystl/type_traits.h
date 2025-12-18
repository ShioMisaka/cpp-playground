#pragma once

namespace mystl
{
template <typename T>
struct remove_reference { using type = T; };

// 特化: 去掉左值引用 
template <typename T>
struct remove_reference<T&> { using type = T; };

// 特化: 去掉右值引用 
template <typename T>
struct remove_reference<T&&> { using type = T; };

template <typename T>
using remove_reference_t = typename remove_reference<T>::type;
}