#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>

// ============================================
// volatile 关键词的用法与误区
// ============================================

// --------------------------------------------
// 1. volatile 的基本含义
// --------------------------------------------
// 告诉编译器该变量可能被外部因素（如硬件、其他线程）修改
// 编译器不会对该变量的访问进行优化（如缓存到寄存器）

void test01_basic_volatile() {
    std::cout << "=== volatile 基本含义 ===" << std::endl;

    // 普通变量
    int normal = 0;
    // volatile 变量
    volatile int vol = 0;

    // 对于普通变量，编译器可能会优化为直接使用寄存器中的值
    // 对于 volatile 变量，每次访问都会从内存中读取

    std::cout << "普通变量大小: " << sizeof(normal) << " 字节" << std::endl;
    std::cout << "volatile 变量大小: " << sizeof(vol) << " 字节" << std::endl;
    std::cout << std::endl;
}

// --------------------------------------------
// 2. volatile 与编译器优化
// --------------------------------------------
// 防止编译器进行某些优化

void test02_optimization_prevention() {
    std::cout << "=== volatile 防止编译器优化 ===" << std::endl;

    volatile int x = 0;

    // 没有 volatile，编译器可能优化为：
    // x = 1000000;  // 直接赋值最终值
    for (int i = 0; i < 1000000; ++i) {
        x++;  // volatile 确保每次循环都真正执行
    }

    std::cout << "循环结束后 x = " << x << std::endl;

    // 对比普通变量可能被优化的情况
    int y = 0;
    for (int i = 0; i < 1000000; ++i) {
        y++;  // 可能被优化
    }
    std::cout << "普通循环 y = " << y << std::endl;
    std::cout << std::endl;
}

// --------------------------------------------
// 3. 常见误解：volatile 不是线程安全的！
// --------------------------------------------
// volatile 不能保证原子性、可见性、有序性
// 多线程环境下应使用 std::atomic

// 错误示例：使用 volatile 进行线程同步（不安全！）
volatile int unsafeCounter = 0;

void unsafeIncrement() {
    for (int i = 0; i < 100000; ++i) {
        unsafeCounter++;  // 非原子操作，存在竞态条件
    }
}

void test03_volatile_not_thread_safe() {
    std::cout << "=== volatile 不是线程安全的 ===" << std::endl;

    unsafeCounter = 0;

    std::thread t1(unsafeIncrement);
    std::thread t2(unsafeIncrement);

    t1.join();
    t2.join();

    std::cout << "使用 volatile 的结果（非预期）: " << unsafeCounter << std::endl;
    std::cout << "预期值: 200000，实际值可能小于预期" << std::endl;
    std::cout << std::endl;
}

// --------------------------------------------
// 4. 正确做法：使用 std::atomic
// --------------------------------------------
// 多线程同步应使用原子操作

std::atomic<int> safeCounter{0};

void safeIncrement() {
    for (int i = 0; i < 100000; ++i) {
        safeCounter++;  // 原子操作，线程安全
    }
}

void test04_atomic_thread_safe() {
    std::cout << "=== 使用 std::atomic 保证线程安全 ===" << std::endl;

    safeCounter = 0;

    std::thread t1(safeIncrement);
    std::thread t2(safeIncrement);

    t1.join();
    t2.join();

    std::cout << "使用 atomic 的结果: " << safeCounter << std::endl;
    std::cout << "结果正确：200000" << std::endl;
    std::cout << std::endl;
}

// --------------------------------------------
// 5. volatile 的正确使用场景
// --------------------------------------------
// 1. 内存映射 I/O（MMIO）
// 2. 信号处理程序中的变量
// 3. 与硬件交互的场景

// 模拟硬件寄存器
class HardwareRegister {
private:
    // 硬件寄存器的值可能由硬件异步修改
    volatile uint32_t* reg;
public:
    HardwareRegister(uint32_t address) : reg(reinterpret_cast<uint32_t*>(address)) {}

    // 每次读取都从硬件获取最新值
    uint32_t read() const {
        return *reg;
    }

    // 每次写入都直接写入硬件
    void write(uint32_t value) {
        *reg = value;
    }

    // 轮询等待硬件就绪
    void waitForReady() {
        while (!(*reg & 0x01)) {
            // volatile 确保每次循环都从寄存器重新读取
            // 编译器不会优化为只读一次
        }
    }
};

void test05_volatile_hardware_io() {
    std::cout << "=== volatile 的正确场景：硬件 I/O ===" << std::endl;

    // 模拟硬件状态寄存器
    volatile int hardwareStatus = 0;

    // 模拟硬件状态变化
    std::thread simulator([&hardwareStatus]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        hardwareStatus = 1;  // 硬件就绪
    });

    // 轮询等待
    std::cout << "等待硬件就绪..." << std::endl;
    int timeout = 0;
    while (hardwareStatus == 0 && timeout < 1000) {
        timeout++;
    }

    if (hardwareStatus) {
        std::cout << "硬件已就绪！" << std::endl;
    } else {
        std::cout << "等待超时" << std::endl;
    }

    simulator.join();
    std::cout << std::endl;
}

// --------------------------------------------
// 6. volatile 与 const 的组合
// --------------------------------------------
// const volatile：值不能被程序修改，但可能被外部改变

void test06_const_volatile() {
    std::cout << "=== const volatile 组合 ===" << std::endl;

    // const volatile 变量
    const volatile int cv = 42;

    // cv = 10;  // 错误：不能修改
    std::cout << "const volatile 值: " << cv << std::endl;

    // 典型应用：只读硬件寄存器
    // 程序只能读取，不能写入，但值可能被硬件改变

    std::cout << std::endl;
}

// --------------------------------------------
// 7. volatile 指针
// --------------------------------------------
// volatile 位置不同，含义不同

void test07_volatile_pointers() {
    std::cout << "=== volatile 指针 ===" << std::endl;

    int x = 10, y = 20;

    // 1. 指向 volatile int 的指针
    // 指向的内容是 volatile 的
    volatile int* p1 = &x;
    std::cout << "指向 volatile int 的指针: " << *p1 << std::endl;

    // 2. volatile 指针（指针本身是 volatile）
    // 指针地址不能被优化
    int* volatile p2 = &x;
    std::cout << "volatile 指针: " << *p2 << std::endl;

    // 3. 两者都是 volatile
    volatile int* volatile p3 = &x;
    std::cout << "两者都是 volatile: " << *p3 << std::endl;

    std::cout << std::endl;
}

// --------------------------------------------
// volatile 关键词总结
// --------------------------------------------
/*
 * volatile 的作用：
 * - 禁止编译器对变量访问进行优化
 * - 每次读写都直接访问内存，不使用缓存
 *
 * 适用场景：
 * 1. 硬件编程：内存映射 I/O、硬件寄存器
 * 2. 中断服务程序：与主程序共享的变量
 * 3. 信号处理：信号处理程序中访问的变量
 *
 * 常见误区：
 * 1. volatile 不能保证线程安全
 * 2. volatile 不能替代互斥锁或原子操作
 * 3. 多线程同步应使用 std::atomic、std::mutex 等
 *
 * volatile vs 原子操作：
 * - volatile: 只影响编译器优化，不涉及 CPU 指令
 * - atomic: 保证操作的原子性、可见性、有序性
 */

int main() {
    test01_basic_volatile();
    test02_optimization_prevention();
    test03_volatile_not_thread_safe();
    test04_atomic_thread_safe();
    test05_volatile_hardware_io();
    test06_const_volatile();
    test07_volatile_pointers();

    std::cout << "============================================" << std::endl;
    std::cout << "重要提示：volatile 不等于线程安全！" << std::endl;
    std::cout << "多线程场景请使用 std::atomic 或 std::mutex" << std::endl;
    std::cout << "============================================" << std::endl;

    return 0;
}
