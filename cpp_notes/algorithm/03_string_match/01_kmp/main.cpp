#include <iostream>

#include <cstdio>
#include <type_traits>
#include <string>

#include "kmp.hpp"

void test01()
{
	using std::string;
	string str = "Hello world";
	// std::cout << str.find("o") << std::endl;

	size_t pos =  kmp_search(str, "world");
	std::cout << pos << std::endl;

	std::string s1 = "ABABABAABABAAABABAA";
	std::string s2 = "ABABAAABABAA";

	size_t pos2 = kmp_search(s1, s2);
	std::cout <<  "pos2: " << pos2 << std::endl;
}


int main() {
    std::cout << "hello world" << std::endl;
    return 0;
}