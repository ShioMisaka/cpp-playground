#include <iostream>

// ============================================
// static 关键词的几种用法
// ============================================

// --------------------------------------------
// 1. 静态局部变量 (Static Local Variables)
// --------------------------------------------
// 在函数内部使用 static，变量的值在函数调用之间保持不变
// 只在第一次调用时初始化，之后跳过初始化

void counter() {
    // 静态局部变量：只初始化一次
    static int count = 0;
    count++;
    std::cout << "函数调用次数: " << count << std::endl;
}

void test01_static_local() {
    std::cout << "=== 静态局部变量 ===" << std::endl;
    counter(); // 输出: 1
    counter(); // 输出: 2
    counter(); // 输出: 3
    std::cout << std::endl;
}

// ✅ 静态局部变量适用于需要保持状态的场景
// ✅ 避免使用全局变量，保持变量作用域在函数内部

// --------------------------------------------
// 2. 静态全局变量/函数 (Static Global Variables/Functions)
// --------------------------------------------
// 限制变量/函数的作用域仅在当前文件内（内部链接）

static int globalValue = 100;  // 只能在当前文件访问

static void helperFunction() {
    std::cout << "这是静态函数，只能在当前文件调用" << std::endl;
}

void test02_static_global() {
    std::cout << "=== 静态全局变量/函数 ===" << std::endl;
    std::cout << "静态全局变量: " << globalValue << std::endl;
    helperFunction();
    std::cout << std::endl;
}

// ✅ 避免命名冲突，尤其在大型项目中
// ✅ 实现信息隐藏，类似 private 的效果

// --------------------------------------------
// 3. 静态成员变量 (Static Member Variables)
// --------------------------------------------
// 类的所有对象共享同一个静态成员变量

class MyClass {
public:
    // 静态成员变量声明
    static int objectCount;

    MyClass() {
        objectCount++;  // 每创建一个对象，计数加1
        std::cout << "构造对象 #" << objectCount << std::endl;
    }

    ~MyClass() {
        objectCount--;  // 销毁对象，计数减1
        std::cout << "析构对象，剩余: " << objectCount << std::endl;
    }

    // 静态成员函数可以访问静态成员变量
    static int getCount() {
        return objectCount;
    }
};

// 静态成员变量必须在类外定义（分配内存）
int MyClass::objectCount = 0;

void test03_static_member() {
    std::cout << "=== 静态成员变量 ===" << std::endl;
    std::cout << "初始对象数: " << MyClass::getCount() << std::endl;

    {
        MyClass obj1;
        MyClass obj2;
        std::cout << "当前对象数: " << MyClass::getCount() << std::endl;
    }  // obj1, obj2 离开作用域被销毁

    std::cout << "作用域结束后对象数: " << MyClass::getCount() << std::endl;

    MyClass obj3;
    std::cout << "最终对象数: " << MyClass::objectCount << std::endl;
    std::cout << std::endl;
}

// ✅ 静态成员变量用于统计对象数量、共享配置等
// ✅ 静态成员变量不属于任何特定对象，而是属于类本身

// --------------------------------------------
// 4. 静态成员函数 (Static Member Functions)
// --------------------------------------------
// 可以通过类名直接调用，不需要对象实例
// 不能访问非静态成员变量或非静态成员函数

class Utility {
public:
    // 静态成员函数
    static int add(int a, int b) {
        return a + b;
    }

    // 非静态成员函数
    void multiply(int a, int b) {
        // 静态函数可以调用非静态函数吗？不能！
        // 静态成员函数没有 this 指针
        std::cout << "乘积: " << a * b << std::endl;
    }
};

void test04_static_member_function() {
    std::cout << "=== 静态成员函数 ===" << std::endl;

    // 通过类名直接调用（不需要对象）
    int sum = Utility::add(5, 3);
    std::cout << "5 + 3 = " << sum << std::endl;

    // 也可以通过对象调用（但不推荐）
    Utility util;
    std::cout << "10 + 20 = " << util.add(10, 20) << std::endl;

    // 非静态函数需要对象
    util.multiply(4, 5);
    std::cout << std::endl;
}

// ✅ 静态成员函数适合工具函数、工厂方法等
// ✅ 不依赖于对象状态的功能

// --------------------------------------------
// 实际应用示例：单例模式
// --------------------------------------------

class Singleton {
private:
    static Singleton* instance;
    int data;

    // 私有构造函数，防止外部创建实例
    Singleton() : data(0) {}

public:
    // 静态成员函数：获取单例实例
    static Singleton* getInstance() {
        if (instance == nullptr) {
            instance = new Singleton();
        }
        return instance;
    }

    void setData(int value) {
        data = value;
    }

    int getData() const {
        return data;
    }
};

// 静态成员初始化
Singleton* Singleton::instance = nullptr;

void test05_singleton_pattern() {
    std::cout << "=== 单例模式示例 ===" << std::endl;

    Singleton* s1 = Singleton::getInstance();
    Singleton* s2 = Singleton::getInstance();

    std::cout << "s1 地址: " << s1 << std::endl;
    std::cout << "s2 地址: " << s2 << std::endl;
    std::cout << "是同一个对象? " << (s1 == s2 ? "是" : "否") << std::endl;

    s1->setData(42);
    std::cout << "通过 s2 获取数据: " << s2->getData() << std::endl;
    std::cout << std::endl;
}

// --------------------------------------------
// static 关键词总结
// --------------------------------------------
/*
 * 1. 静态局部变量: 函数内保持状态，只初始化一次
 * 2. 静态全局变量/函数: 限制作用域到当前文件
 * 3. 静态成员变量: 所有对象共享，属于类
 * 4. 静态成员函数: 通过类名调用，无 this 指针
 *
 * 内存位置：
 * - 静态变量存储在静态存储区，程序整个运行期间都存在
 * - 全局静态变量在程序启动时分配，程序结束时释放
 */

int main() {
    test01_static_local();
    test02_static_global();
    test03_static_member();
    test04_static_member_function();
    test05_singleton_pattern();

    return 0;
}