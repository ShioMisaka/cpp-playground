#include <iostream>

// 1. 定义常量变量
const int MAX_SIZE = 100;
// MAX_SIZE = 200; // ❌ 错误！不能修改 const 变量

// ✅ 比 #define MAX_SIZE 100 更安全（有类型检查，作用域可控）。

// 2. 函数参数（防止意外修改）
void printValue(const int x) {
    // x = 10; // ❌ 不允许修改
    std::cout << x << std::endl;
}

// 更常见的是用于指针或引用参数，这也既可以接收常量，以避免更改了原有值：
void printString(const std::string& str) {
    // str += "!"; // ❌ 不能修改
    std::cout << str << std::endl;
}

void test01()
{
    printString("一个常量字符串");
}

// ✅ 既避免了拷贝开销（用引用），又保证了安全性（用 const）。

// 3. 指针与 const（重点！容易混淆）
void test02()
{
    int x = 0;
    // 指针指向的内容不可变
    const int *p1 = &x; // 不能通过 p 修改 x（*p = 10 ❌）
    int const *p2 = &x; // 等价于 const int *

    // 指针本身不可变（常量指针）
    int* const p3 = &x;        // p 不能指向别的变量（p = &y ❌）

    const int* const p4 = &x;  // 内容和指针都不可变
}

// 类的成员函数（const 成员函数）
// 表示该函数不会修改对象的状态：
class MyClass {
private:
    int value;
public:
    MyClass() : value(0) {};

    int getValue() const {  // ✅ 不会修改成员变量
        return value;
    }
    
    void setValue(int v) {
        value = v;          // ✅ 可以修改
    }
};

void test03()
{
    const MyClass obj;
    obj.getValue();  // ✅
    // obj.setValue(5); // ❌ 错误！
    // obj.getName() = "Alice"; // ❌ 如果返回 const 引用，这行会报错
}


int main() {
    test01();
    return 0;
}