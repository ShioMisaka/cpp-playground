#pragma once

#include <utility>
#include <iostream>
#include <type_traits>

#include "type_traits.h"

namespace mystl
{

#ifdef __GNUG__
#include <cxxabi.h>
#endif

template<typename T>
std::string type_name() {
    using TR = typename std::remove_reference<T>::type;
    std::string name = typeid(TR).name();

#ifdef __GNUG__ // GCC / Clang 下进行 demangle（解码）
    int status;
    char *demangled = abi::__cxa_demangle(name.c_str(), 0, 0, &status);
    if (status == 0 && demangled) {
        name = demangled;
        std::free(demangled);
    }
#endif

    if (std::is_const<TR>::value) name += " const";
    if (std::is_volatile<TR>::value) name += " volatile";
    if (std::is_lvalue_reference<T>::value) name += "&";
    else if (std::is_rvalue_reference<T>::value) name += "&&";

    return name;
}


// 将对象强制转换成右值引用，从而允许它触发“移动构造”或“移动赋值”。
template <typename T>
typename mystl::remove_reference<T>::type&& move(T&& t)
{
	return static_cast<typename mystl::remove_reference<T>::type&&>(t);
}

// 完美转发
template <typename T>
T&& forward(typename mystl::remove_reference_t<T>& arg) noexcept
 {
	return static_cast<T&&>(arg);
}

template <typename T>
T&& forward(typename mystl::remove_reference<T>::type&& arg) noexcept {
	return static_cast<T &&>(arg);
}


// 打印 container 的信息
template<typename Iter>
void print_container_data(Iter begin, Iter end)
{
	std::cout << "[ ";
	for (auto it = begin; it != end; ++it)
		std::cout << *it << " ";
	std::cout << "]"<< std::endl;
}

template<typename _Cntr, decltype(std::declval<_Cntr>().capacity(), 
                                  std::declval<_Cntr>().size(), 
                                  std::declval<_Cntr>().begin(),
                                  std::declval<_Cntr>().end(), 0) = 0>
void print_container_info(const _Cntr& cntr, size_t limit = 15)
{
	std::cout << cntr.data() << " s:c = " << cntr.size()<< ":"<< cntr.capacity() << std::endl;
	print_container_data(cntr.begin(), cntr.end());
}


}