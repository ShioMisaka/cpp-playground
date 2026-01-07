#include <iostream>

// ============================================
// inline 关键词的几种用法
// ============================================

// --------------------------------------------
// 1. 内联函数 (Inline Functions)
// --------------------------------------------
// inline 是对编译器的建议，请求将函数代码直接插入调用点
// 目的：消除函数调用的开销（参数传递、栈帧创建等）

// 普通函数（编译器可能会内联，但不保证）
int addNormal(int a, int b) {
    return a + b;
}

// 内联函数（明确建议编译器内联）
inline int addInline(int a, int b) {
    return a + b;
}

// 编译器可以选择忽略 inline 关键字
// 现代编译器有自己的优化策略，通常会自动内联小函数

void test01_basic_inline() {
    std::cout << "=== 基本内联函数 ===" << std::endl;

    int result1 = addNormal(3, 4);
    int result2 = addInline(5, 6);

    std::cout << "普通函数结果: " << result1 << std::endl;
    std::cout << "内联函数结果: " << result2 << std::endl;
    std::cout << std::endl;
}

// ✅ 内联函数适合短小、频繁调用的函数
// ✅ 现代编译器会自动优化，inline 更多是提示作用

// --------------------------------------------
// 2. 类中的内联函数 (Inline Member Functions)
// --------------------------------------------
// 在类内部定义的成员函数默认是内联的

class Calculator {
public:
    // 在类内定义 - 隐式内联
    int multiply(int a, int b) {
        return a * b;
    }

    // 在类内声明
    int divide(int a, int b);

    // 使用 inline 关键字的成员函数
    inline int square(int x) {
        return x * x;
    }
};

// 在类外定义，但使用 inline 关键字
inline int Calculator::divide(int a, int b) {
    if (b == 0) {
        std::cout << "警告：除数为零！" << std::endl;
        return 0;
    }
    return a / b;
}

void test02_class_inline() {
    std::cout << "=== 类中内联函数 ===" << std::endl;

    Calculator calc;
    std::cout << "3 * 4 = " << calc.multiply(3, 4) << std::endl;
    std::cout << "10 / 2 = " << calc.divide(10, 2) << std::endl;
    std::cout << "5 的平方 = " << calc.square(5) << std::endl;
    std::cout << std::endl;
}

// ✅ 类内定义的函数自动隐式内联
// ✅ 适合简短的单行或几行函数

// --------------------------------------------
// 3. inline 与头文件 (避免 ODR 违规)
// --------------------------------------------
// ODR (One Definition Rule): 一个实体在整个程序中只能有一个定义
// inline 允许在多个翻译单元中定义相同的函数/变量

// math_utils.h 中的内联函数示例
// 如果在头文件中定义普通函数，会导致链接错误（多重定义）
// 但 inline 函数可以在多个 .cpp 文件包含同一个头文件

inline int cube(int x) {
    return x * x * x;
}

// 假设这个函数在头文件中被多个 cpp 文件包含
// 没有 inline 会导致链接错误
// 有 inline 则每个编译单元都可以有自己的定义

void test03_header_inline() {
    std::cout << "=== inline 解决 ODR 问题 ===" << std::endl;

    // 这个函数可以被多个编译单元使用而不会产生链接错误
    std::cout << "3 的立方 = " << cube(3) << std::endl;
    std::cout << "4 的立方 = " << cube(4) << std::endl;
    std::cout << std::endl;
}

// ✅ inline 是头文件中定义函数的必要条件
// ✅ 允许模板函数在头文件中实现

// --------------------------------------------
// 4. inline 变量 (C++17)
// --------------------------------------------
// C++17 引入，用于在头文件中定义变量而不会导致 ODR 违规

// 普通静态常量成员变量（旧方式）
class OldStyle {
public:
    static const int MAX_SIZE = 100;  // 声明，整数类型可以类内初始化
    // static const double PI = 3.14159;  // 非整数类型需要类外定义
};

// 使用 inline 变量（C++17 新方式）
class NewStyle {
public:
    inline static const int MAX_SIZE = 100;      // C++17
    inline static const double PI = 3.14159;     // C++17
    inline static int counter = 0;               // C++17，甚至可以是非 const
};

// 全局 inline 变量
inline int globalCounter = 0;  // 可以在多个翻译单元中使用

void test04_inline_variable() {
    std::cout << "=== inline 变量 (C++17) ===" << std::endl;

    std::cout << "OldStyle::MAX_SIZE = " << OldStyle::MAX_SIZE << std::endl;
    std::cout << "NewStyle::MAX_SIZE = " << NewStyle::MAX_SIZE << std::endl;
    std::cout << "NewStyle::PI = " << NewStyle::PI << std::endl;

    NewStyle::counter = 42;
    std::cout << "NewStyle::counter = " << NewStyle::counter << std::endl;
    std::cout << std::endl;
}

// ✅ inline 变量简化了静态成员变量的定义
// ✅ 不需要在类外再定义一次

// --------------------------------------------
// 5. inline 命名空间
// --------------------------------------------
// 用于版本控制和库的演进
// inline 命名空间的成员会被外层命名空间直接可见

// 版本 1 的库
namespace V1 {
    int calculate(int x) {
        return x * 2;
    }
}

// 当前版本（推荐使用）
inline namespace V2 {
    int calculate(int x) {
        return x * 3;
    }

    int newFeature(int x) {
        return x + 10;
    }
}

void test05_inline_namespace() {
    std::cout << "=== inline 命名空间 ===" << std::endl;

    // 直接调用，不需要指定命名空间（因为 V2 是 inline）
    std::cout << "calculate(5) = " << calculate(5) << std::endl;
    std::cout << "newFeature(5) = " << newFeature(5) << std::endl;

    // 显式调用旧版本
    std::cout << "V1::calculate(5) = " << V1::calculate(5) << std::endl;
    std::cout << std::endl;
}

// ✅ inline namespace 用于库版本管理
// ✅ 使用者默认使用最新版本，也可以显式使用旧版本

// --------------------------------------------
// 6. constexpr 与 inline
// --------------------------------------------
// constexpr 函数隐式内联，且可以在编译时求值

constexpr int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);
}

void test06_constexpr_inline() {
    std::cout << "=== constexpr 隐式内联 ===" << std::endl;

    // 编译时计算
    constexpr int result = factorial(5);
    std::cout << "5! = " << result << std::endl;

    // 运行时计算（如果参数不是常量）
    int n = 6;
    std::cout << "6! = " << factorial(n) << std::endl;
    std::cout << std::endl;
}

// ✅ constexpr 函数自动是内联的
// ✅ 优先使用 constexpr 而不是 inline

// --------------------------------------------
// 7. 何时使用 inline
// --------------------------------------------

// 适合 inline 的场景：
class Example {
public:
    // ✅ 简短的 getter/setter
    inline int getValue() const { return m_value; }
    inline void setValue(int value) { m_value = value; }

    // ✅ 简单的数学运算
    inline int add(int a, int b) { return a + b; }

    // ❌ 复杂的逻辑不适合 inline
    void complexOperation() {
        // 大量代码...
        // 编译器可能会忽略 inline 建议
        for (int i = 0; i < 1000; ++i) {
            // 复杂逻辑
        }
    }

private:
    int m_value;
};

void test07_when_to_use() {
    std::cout << "=== 何时使用 inline ===" << std::endl;

    Example ex;
    ex.setValue(42);
    std::cout << "Getter 返回: " << ex.getValue() << std::endl;
    std::cout << std::endl;
}

// --------------------------------------------
// inline 关键词总结
// --------------------------------------------
/*
 * 1. 内联函数: 建议编译器将函数代码插入调用点
 * 2. 类内定义: 成员函数在类内定义自动隐式内联
 * 3. ODR 解决: 允许在头文件中定义函数，避免多重定义
 * 4. inline 变量 (C++17): 在头文件中定义变量
 * 5. inline 命名空间: 用于库版本管理
 * 6. constexpr: 隐式内联，优先使用
 *
 * 注意事项：
 * - inline 只是建议，编译器可以忽略
 * - 现代编译器会自动优化，不需要显式 inline
 * - 过度使用会增加代码体积（代码膨胀）
 * - 适合短小、频繁调用的函数
 * - 复杂函数即使 inline 也可能不会被内联
 *
 * 编译器优化：
 * - 开启优化后，编译器会自动决定是否内联
 * - Link Time Optimization (LTO) 可以跨编译单元内联
 */

int main() {
    test01_basic_inline();
    test02_class_inline();
    test03_header_inline();
    test04_inline_variable();
    test05_inline_namespace();
    test06_constexpr_inline();
    test07_when_to_use();

    return 0;
}
