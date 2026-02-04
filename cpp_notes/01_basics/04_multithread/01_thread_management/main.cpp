#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <functional>
#include <stop_token>

// ============================================
// C++ 多线程基础管理 (Thread Management)
// ============================================

// --------------------------------------------
// 1. std::thread 基础用法
// --------------------------------------------
// std::thread 是 C++11 引入的线程类，用于创建和管理线程
// 线程在构造时启动，在析构前必须被 join() 或 detach()

void workerFunction(int id) {
    std::cout << "  [Worker " << id << "] 线程 ID: "
              << std::this_thread::get_id() << std::endl;
    std::cout << "  [Worker " << id << "] 开始工作..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "  [Worker " << id << "] 工作完成！" << std::endl;
}

void test01_basic_thread() {
    std::cout << "=== std::thread 基础用法 ===" << std::endl;
    std::cout << "[主线程] 线程 ID: " << std::this_thread::get_id() << std::endl;

    // 创建多个线程
    std::thread t1(workerFunction, 1);
    std::thread t2(workerFunction, 2);
    std::thread t3(workerFunction, 3);

    std::cout << "[主线程] 已创建 3 个工作线程" << std::endl;

    // join(): 阻塞当前线程，直到该线程执行完成
    // 必须在 thread 对象析构前调用 join() 或 detach()
    t1.join();
    std::cout << "[主线程] Thread 1 已 join" << std::endl;
    t2.join();
    std::cout << "[主线程] Thread 2 已 join" << std::endl;
    t3.join();
    std::cout << "[主线程] Thread 3 已 join" << std::endl;

    std::cout << "[主线程] 所有线程执行完毕" << std::endl;
    std::cout << std::endl;
}

// --------------------------------------------
// 2. join() vs detach() 的区别
// --------------------------------------------
// join(): 等待线程完成（阻塞）
// detach(): 分离线程，让其独立运行（守护线程）
// 注意：一旦 detach，就无法再 join 该线程

void detachedWorker() {
    std::cout << "  [分离线程] 开始执行，将独立运行..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    std::cout << "  [分离线程] 执行中..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "  [分离线程] 即将完成" << std::endl;
}

void test02_join_vs_detach() {
    std::cout << "=== join() vs detach() ===" << std::endl;

    // 示例 1: 使用 join()
    std::cout << "\n[示例 1] 使用 join():" << std::endl;
    std::thread jThread([]() {
        std::cout << "  [Join 线程] 开始工作..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        std::cout << "  [Join 线程] 完成！" << std::endl;
    });

    std::cout << "[主线程] 等待 join 线程完成..." << std::endl;
    jThread.join();  // 阻塞直到线程完成
    std::cout << "[主线程] join 线程已确认完成" << std::endl;

    // 示例 2: 使用 detach()
    std::cout << "\n[示例 2] 使用 detach():" << std::endl;
    std::thread dThread(detachedWorker);

    std::cout << "[主线程] 分离线程，让它独立运行" << std::endl;
    dThread.detach();  // 分离后线程独立运行

    // 检查线程是否可 join
    if (dThread.joinable()) {
        std::cout << "[主线程] 线程仍可 join" << std::endl;
    } else {
        std::cout << "[主线程] 线程已 detach，无法 join" << std::endl;
    }

    // 等待分离线程完成（只是为了演示效果）
    std::this_thread::sleep_for(std::chrono::milliseconds(1200));
    std::cout << "[主线程] 分离线程应该已完成（后台运行）" << std::endl;

    // 示例 3: 忘记 join 或 detach 的后果（程序崩溃）
    std::cout << "\n[示例 3] 正确处理线程生命周期:" << std::endl;
    std::thread properThread([]() {
        std::cout << "  [正确线程] 工作..." << std::endl;
    });

    // 总是调用 join() 或 detach()
    if (properThread.joinable()) {
        properThread.join();
        std::cout << "[主线程] 线程已正确处理" << std::endl;
    }

    std::cout << std::endl;
}

// --------------------------------------------
// 3. C++20 std::jthread: 自动 join 的智能线程
// --------------------------------------------
// std::jthread 在析构时会自动调用 join()
// 支持中断令牌 (stop_token) 用于优雅地请求线程停止
// 这是 RAII (Resource Acquisition Is Initialization) 的应用

void jthreadWorker(std::stop_token stop, int id) {
    std::cout << "  [JThread " << id << "] 开始工作" << std::endl;

    int counter = 0;
    // 检查是否收到停止请求
    while (!stop.stop_requested()) {
        std::cout << "  [JThread " << id << "] 工作中... " << ++counter << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    std::cout << "  [JThread " << id << "] 收到停止信号，正在清理..." << std::endl;
    std::cout << "  [JThread " << id << "] 退出" << std::endl;
}

void test03_jthread_auto_join() {
    std::cout << "=== C++20 std::jthread: 自动 join ===" << std::endl;

    // 示例 1: 自动 join（RAII）
    std::cout << "\n[示例 1] 自动 join:" << std::endl;
    {
        std::jthread jt1([](int id) {
            std::cout << "  [JThread " << id << "] 执行任务..." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            std::cout << "  [JThread " << id << "] 完成" << std::endl;
        }, 1);

        std::cout << "[主线程] JThread 创建，将在作用域结束时自动 join" << std::endl;
        // 不需要显式调用 join()，析构时自动调用
    }
    std::cout << "[主线程] 已离开作用域，jthread 自动 join 完成" << std::endl;

    // 示例 2: 使用 stop_token 请求停止
    std::cout << "\n[示例 2] 使用 stop_token 优雅停止:" << std::endl;
    std::jthread jt2(jthreadWorker, 2);

    std::cout << "[主线程] 让线程运行 1 秒..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "[主线程] 请求线程停止..." << std::endl;
    jt2.request_stop();  // 请求线程停止

    // jthread 析构时会自动 join，等待线程响应停止请求并退出
    std::cout << "[主线程] 等待线程响应停止请求..." << std::endl;
    // 自动 join 发生在这里

    std::cout << "[主线程] JThread 已优雅退出" << std::endl;

    // 示例 3: std::thread vs std::jthread 对比
    std::cout << "\n[示例 3] std::thread vs std::jthread:" << std::endl;
    std::cout << "[主线程] std::thread: 必须手动 join，否则程序可能崩溃" << std::endl;
    std::cout << "[主线程] std::jthread: 自动 join，更安全（RAII）" << std::endl;
    std::cout << "[主线程] std::jthread: 支持 stop_token 通信机制" << std::endl;

    std::cout << std::endl;
}

// --------------------------------------------
// 4. 参数传递：为什么需要 std::ref
// --------------------------------------------
// 线程函数的参数默认按值（拷贝）传递
// 要传递引用，必须使用 std::ref() 或 std::cref()
// 这是因为线程构造时会拷贝参数到新的线程栈

void passByValue(int x) {
    std::cout << "  [值传递] 接收到: " << x << std::endl;
    x *= 2;  // 修改的是副本，不影响原变量
    std::cout << "  [值传递] 修改后: " << x << std::endl;
}

void passByRef(int& x) {
    std::cout << "  [引用传递] 接收到: " << x << std::endl;
    x *= 2;  // 修改的是原变量
    std::cout << "  [引用传递] 修改后: " << x << std::endl;
}

void test04_argument_passing() {
    std::cout << "=== 参数传递：std::ref 的使用 ===" << std::endl;

    // 示例 1: 按值传递（默认行为）
    std::cout << "\n[示例 1] 按值传递（默认）:" << std::endl;
    int value1 = 10;
    std::cout << "[主线程] 原值: " << value1 << std::endl;

    std::thread t1(passByValue, value1);
    t1.join();

    std::cout << "[主线程] 线程结束后, value1 = " << value1 << " (未改变)" << std::endl;

    // 示例 2: 使用 std::ref 按引用传递
    std::cout << "\n[示例 2] 按引用传递（使用 std::ref）:" << std::endl;
    int value2 = 10;
    std::cout << "[主线程] 原值: " << value2 << std::endl;

    std::thread t2(passByRef, std::ref(value2));
    t2.join();

    std::cout << "[主线程] 线程结束后, value2 = " << value2 << " (已改变)" << std::endl;

    // 示例 3: 错误示范 - 不使用 std::ref 传递引用
    std::cout << "\n[示例 3] 错误示范（编译错误）:" << std::endl;
    std::cout << "[主线程] std::thread t3(passByRef, value2); // 编译错误！" << std::endl;
    std::cout << "[主线程] 原因: value2 是 int 类型，无法转换为 int&" << std::endl;
    std::cout << "[主线程] 必须使用 std::ref(value2) 显式传递引用" << std::endl;

    // 示例 4: 传递类成员函数
    std::cout << "\n[示例 4] 传递类成员函数:" << std::endl;

    class Worker {
    private:
        std::string name_;
    public:
        Worker(std::string name) : name_(name) {}

        void task(int count) {
            for (int i = 0; i < count; ++i) {
                std::cout << "  [" << name_ << "] 执行任务 " << (i + 1) << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }
    };

    Worker w("Worker-1");
    std::thread t3(&Worker::task, &w, 3);  // &w 传递对象指针
    t3.join();

    // 示例 5: 传递多个参数
    std::cout << "\n[示例 5] 传递多个参数:" << std::endl;
    auto multiArgWorker = [](int a, const std::string& b, double c) {
        std::cout << "  [多参数线程] a=" << a << ", b=" << b << ", c=" << c << std::endl;
    };

    std::thread t4(multiArgWorker, 42, "hello", 3.14);
    t4.join();

    std::cout << std::endl;
}

// --------------------------------------------
// 5. 线程管理的最佳实践
// --------------------------------------------
// 展示一些实用的线程管理技巧

void test05_best_practices() {
    std::cout << "=== 线程管理最佳实践 ===" << std::endl;

    // 实践 1: 使用 joinable() 检查
    std::cout << "\n[实践 1] 使用 joinable() 检查:" << std::endl;
    std::thread t1([]() {
        std::cout << "  [线程] 工作中..." << std::endl;
    });

    if (t1.joinable()) {
        t1.join();
        std::cout << "[主线程] 线程已安全 join" << std::endl;
    }

    // 实践 2: 使用 RAII 确保线程被 join
    std::cout << "\n[实践 2] RAII 包装器（在 C++17 之前）:" << std::endl;
    std::cout << "[主线程] 使用自定义类确保析构时 join" << std::endl;
    std::cout << "[主线程] C++20 推荐直接使用 std::jthread" << std::endl;

    // 实践 3: 硬件并发度
    std::cout << "\n[实践 3] 查询硬件并发能力:" << std::endl;
    unsigned int hardwareThreads = std::thread::hardware_concurrency();
    std::cout << "[主线程] 系统支持 " << hardwareThreads << " 个并发线程" << std::endl;

    // 实践 4: 获取当前线程 ID
    std::cout << "\n[实践 4] 线程 ID 的用途:" << std::endl;
    std::cout << "[主线程] 主线程 ID: " << std::this_thread::get_id() << std::endl;
    std::thread t5([]() {
        std::cout << "  [工作线程] 线程 ID: " << std::this_thread::get_id() << std::endl;
    });
    t5.join();
    std::cout << "[主线程] 线程 ID 可用于日志、调试" << std::endl;

    std::cout << std::endl;
}

// --------------------------------------------
// 线程管理总结
// --------------------------------------------
/*
 * std::thread 基础:
 * - 构造时启动线程，传入可调用对象和参数
 * - 必须在析构前调用 join() 或 detach()
 * - join() 阻塞等待线程完成，detach() 让线程独立运行
 *
 * join() vs detach():
 * - join(): 等待线程完成，确保线程执行完毕后才继续
 * - detach(): 分离线程，让其独立运行（守护线程模式）
 * - 忘记处理会导致程序调用 std::terminate() 终止
 *
 * std::jthread (C++20):
 * - RAII 风格的线程类，析构时自动 join
 * - 支持 stop_token 机制，可请求线程优雅停止
 * - 比 std::thread 更安全，推荐优先使用
 *
 * 参数传递:
 * - 默认按值（拷贝）传递参数到新线程
 * - 需要传递引用时使用 std::ref() 或 std::cref()
 * - 传递成员函数时需要对象指针或引用
 *
 * 最佳实践:
 * - 优先使用 std::jthread（C++20 及以上）
 * - 使用 joinable() 检查再调用 join()
 * - 考虑硬件并发度避免过度创建线程
 * - 使用线程 ID 进行日志和调试
 */

int main() {

    // test01_basic_thread();
    test02_join_vs_detach();
    // test03_jthread_auto_join();
    // test04_argument_passing();
    // test05_best_practices();

    std::cout << "    记住: 优先使用 std::jthread (C++20)" << std::endl;
    std::cout << "    记住: 传递引用需要 std::ref()" << std::endl;
    return 0;
}
