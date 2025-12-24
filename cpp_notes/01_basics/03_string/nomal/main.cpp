#include <iostream>

// string 你了解多少？
// 问题1：在 C++ 中，引号括起来的字符串（如 "hello"）在 C++ 中是什么？
// 问题2："hello" 它是左值（lvalue）还是右值（rvalue）？



// 问题1：在 C++ 中，引号括起来的字符串（如 "hello"）在 C++ 中是什么？
// 它是一个 字符串字面量（string literal），其类型是
char str[6] = "hello";

// 注意：C++11 起，字符串字面量的类型明确为 const char[N]（C 语言中是 char[N]，但修改它是未定义行为；C++ 直接加了 const 更安全）。

// 问题二：它是左值（lvalue）还是右值（rvalue）？
/*答案：字符串字面量是一个左值（lvalue）！

这可能会让很多人感到意外，因为我们通常认为“字面量”是临时的、不可寻址的（比如 42 是纯右值）。但字符串字面量是个例外。
📌 为什么是左值？
字符串字面量在程序中具有固定的内存地址（通常存储在只读数据段 .rodata 中）。
你可以对它取地址（&"hello" 在语法上是合法的，虽然不常用）。
C++ 标准明确规定：字符串字面量是左值（见 [lex.string]/8）。
*/

void test01()
{
    const char* p = "hello";     // 合法：左值可绑定到 const 引用或指针
    const char (&ref)[6] = "hello"; // 合法！左值可绑定到左值引用

    // 以下取地址也合法（虽然很少用）：
    const char (*arrPtr)[6] = &"hello";  // 可以用数组指针指它
    
    // 他们指向同一块地址！！！
    std::cout << (void*)p << '\n';
    std::cout << &ref << '\n';
    std::cout << arrPtr << '\n';
}

void test02()
{
	if (std::is_signed<char>::value) {
		printf("is signed, your computer is x86. \n");
	}
	else {
		printf("is unsigned, your computer is arm. \n");
	}

	const char str[] = "hello\0 111";
	std::cout << str << std::endl;
}

int main() {
    test02();
    return 0;
}