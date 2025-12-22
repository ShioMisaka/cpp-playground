#include <iostream>

/* 指针和引用的区别
    指针：存放对象地址的变量，本身有地址，可以改变指向
    引用：变量的别名，从一而终，不可变，必须初始化
*/

void test01()
{
    int temp = 10;
    int temp2 = 20;

    // 常量指针（底层cosnt）- 不能通过指针修改值
    const int *a = &temp;
    int const *b = &temp; // 等价于上式
    // *a = 0; // ❌ 错误：表达式必须是可以修改的左值

    std::cout << "*a=" << *a << ", *b=" << *b << "\n";

    // 指针常量（顶层const）- 不能改变指向
    int *const p = &temp;
    *p = 9; // 可以通过指针修改值
    // p = &temp2; // ❌ 错误：表达式必须是可以修改的左值

    std::cout << "*p=" << *p << "\n";

    // 引用示例
    int& ref = temp; // 引用必须初始化
    ref = 15; // 直接修改原变量
    // int& ref2; // ❌ 错误：引用必须初始化
    // int& ref3 = nullptr; // ❌ 错误：不存在空引用

    std::cout << "ref=" << ref << "\n";

    // 常量引用
    const int& cref = temp;
    // cref = 20; // ❌ 错误：不能通过常量引用修改值
    temp = 20; // 正确：直接修改原变量
}

int main() {
    std::cout << "hello world" << std::endl;
    return 0;
}