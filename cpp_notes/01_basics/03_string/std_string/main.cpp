#include <iostream>
#include <string>
#include <cstring>

// ============================================
// std::string 基本用法和注意事项
// ============================================

// --------------------------------------------
// 1. 基本构造和初始化
// --------------------------------------------
void test01_construction() {
    std::cout << "=== 构造和初始化 ===" << std::endl;

    // 默认构造：空字符串
    std::string s1;
    std::cout << "s1 (空): \"" << s1 << "\", 长度: " << s1.length() << std::endl;

    // 使用字符串字面量构造
    std::string s2 = "Hello";
    std::string s3("World");
    std::cout << "s2: \"" << s2 << "\"" << std::endl;
    std::cout << "s3: \"" << s3 << "\"" << std::endl;

    // 使用重复字符构造
    std::string s4(5, 'A');
    std::cout << "s4 (5个'A'): \"" << s4 << "\"" << std::endl;

    // 拷贝构造
    std::string s5 = s2;
    std::cout << "s5 (拷贝s2): \"" << s5 << "\"" << std::endl;

    // 使用子串构造
    std::string s6("Hello World", 0, 5);  // 从位置0开始，取5个字符
    std::cout << "s6 (子串): \"" << s6 << "\"" << std::endl;

    // 使用前n个字符构造
    std::string s7("Hello World", 5);  // 只取前5个字符
    std::cout << "s7 (前5字符): \"" << s7 << "\"" << std::endl;

    std::cout << std::endl;
}

// --------------------------------------------
// 2. 基本操作
// --------------------------------------------
void test02_basic_operations() {
    std::cout << "=== 基本操作 ===" << std::endl;

    std::string s = "Hello";

    // 获取长度
    std::cout << "长度: " << s.length() << std::endl;
    std::cout << "大小: " << s.size() << std::endl;   // size() 与 length() 相同

    // 判断是否为空
    std::cout << "是否为空: " << (s.empty() ? "是" : "否") << std::endl;

    // 清空字符串
    s.clear();
    std::cout << "清空后是否为空: " << (s.empty() ? "是" : "否") << std::endl;

    // 重新赋值
    s = "World";
    std::cout << "重新赋值: \"" << s << "\"" << std::endl;

    std::cout << std::endl;
}

// --------------------------------------------
// 3. 元素访问
// --------------------------------------------
void test03_element_access() {
    std::cout << "=== 元素访问 ===" << std::endl;

    std::string s = "Hello";

    // 使用 [] 运算符（不进行边界检查）
    std::cout << "s[0]: " << s[0] << std::endl;
    s[0] = 'h';  // 可以修改
    std::cout << "修改后: " << s << std::endl;

    // 使用 at() 进行边界检查
    try {
        std::cout << "s.at(0): " << s.at(0) << std::endl;
        // s.at(10);  // 抛出 std::out_of_range 异常
    } catch (const std::out_of_range& e) {
        std::cout << "越界访问: " << e.what() << std::endl;
    }

    // 获取首尾字符（C++11）
    std::cout << "首字符: " << s.front() << std::endl;
    std::cout << "尾字符: " << s.back() << std::endl;

    // 获取C风格字符串指针
    const char* cstr = s.c_str();
    std::cout << "C风格字符串: " << cstr << std::endl;

    std::cout << std::endl;
}

// --------------------------------------------
// 4. 字符串拼接
// --------------------------------------------
void test04_concatenation() {
    std::cout << "=== 字符串拼接 ===" << std::endl;

    std::string s1 = "Hello";
    std::string s2 = "World";

    // 使用 + 运算符
    std::string s3 = s1 + " " + s2;
    std::cout << "s1 + \" \" + s2: " << s3 << std::endl;

    // 使用 += 运算符
    s1 += " ";
    s1 += s2;
    std::cout << "s1 += 后: " << s1 << std::endl;

    // 使用 append()
    std::string s4 = "Hello";
    s4.append(" ").append("C++");
    std::cout << "append(): " << s4 << std::endl;

    // 添加单个字符
    s4.push_back('!');
    std::cout << "push_back('!'): " << s4 << std::endl;

    std::cout << std::endl;
}

// --------------------------------------------
// 5. 字符串比较
// --------------------------------------------
void test05_comparison() {
    std::cout << "=== 字符串比较 ===" << std::endl;

    std::string s1 = "apple";
    std::string s2 = "banana";
    std::string s3 = "apple";

    // 使用比较运算符
    std::cout << "s1 == s3: " << (s1 == s3 ? "true" : "false") << std::endl;
    std::cout << "s1 < s2: " << (s1 < s2 ? "true" : "false") << std::endl;

    // 使用 compare() 函数
    // 返回值：0表示相等，<0表示小于，>0表示大于
    int result = s1.compare(s2);
    std::cout << "s1.compare(s2): " << result << std::endl;

    result = s1.compare(s3);
    std::cout << "s1.compare(s3): " << result << std::endl;

    std::cout << std::endl;
}

// --------------------------------------------
// 6. 查找和子串操作
// --------------------------------------------
void test06_find_and_substring() {
    std::cout << "=== 查找和子串 ===" << std::endl;

    std::string s = "Hello World Hello C++";

    // find(): 查找子串位置
    size_t pos = s.find("Hello");
    std::cout << "find(\"Hello\"): " << pos << std::endl;

    // 从指定位置开始查找
    pos = s.find("Hello", 6);  // 从位置6开始查找
    std::cout << "find(\"Hello\", 6): " << pos << std::endl;

    // rfind(): 从后往前查找
    pos = s.rfind("Hello");
    std::cout << "rfind(\"Hello\"): " << pos << std::endl;

    // find_first_of(): 查找任意一个字符
    pos = s.find_first_of("aeiou");
    std::cout << "find_first_of(\"aeiou\"): " << pos << std::endl;

    // find_first_not_of(): 查找第一个不在集合中的字符
    pos = s.find_first_not_of("Helo ");
    std::cout << "find_first_not_of(\"Helo \"): " << pos << std::endl;

    // 判断是否包含子串（C++23前使用find）
    bool contains = s.find("World") != std::string::npos;
    std::cout << "包含 \"World\": " << (contains ? "是" : "否") << std::endl;

    // substr(): 获取子串
    std::string sub = s.substr(6, 5);  // 从位置6开始，取5个字符
    std::cout << "substr(6, 5): \"" << sub << "\"" << std::endl;

    std::cout << std::endl;
}

// --------------------------------------------
// 7. 修改操作
// --------------------------------------------
void test07_modification() {
    std::cout << "=== 修改操作 ===" << std::endl;

    std::string s = "Hello World";

    // insert(): 插入字符串
    s.insert(5, " Beautiful");
    std::cout << "insert: " << s << std::endl;

    // erase(): 删除字符
    s.erase(5, 10);  // 从位置5开始删除10个字符
    std::cout << "erase: " << s << std::endl;

    // replace(): 替换
    s.replace(6, 5, "C++");  // 从位置6开始，替换5个字符为"C++"
    std::cout << "replace: " << s << std::endl;

    // pop_back(): 删除最后一个字符
    s.pop_back();
    std::cout << "pop_back: " << s << std::endl;

    std::cout << std::endl;
}

// --------------------------------------------
// 8. 迭代器遍历
// --------------------------------------------
void test08_iteration() {
    std::cout << "=== 迭代器遍历 ===" << std::endl;

    std::string s = "Hello";

    // 使用下标遍历
    std::cout << "下标遍历: ";
    for (size_t i = 0; i < s.length(); ++i) {
        std::cout << s[i];
    }
    std::cout << std::endl;

    // 使用迭代器遍历
    std::cout << "迭代器遍历: ";
    for (std::string::iterator it = s.begin(); it != s.end(); ++it) {
        std::cout << *it;
    }
    std::cout << std::endl;

    // 使用范围for循环（C++11，推荐）
    std::cout << "范围for: ";
    for (char c : s) {
        std::cout << c;
    }
    std::cout << std::endl;

    // 基于范围的for循环修改
    std::cout << "转大写: ";
    for (char& c : s) {
        c = toupper(c);
    }
    std::cout << s << std::endl;

    std::cout << std::endl;
}

// --------------------------------------------
// 9. 数值转换
// --------------------------------------------
void test09_number_conversion() {
    std::cout << "=== 数值转换 ===" << std::endl;

    // to_string(): 数值转字符串
    int i = 42;
    double d = 3.14;
    std::string s1 = std::to_string(i);
    std::string s2 = std::to_string(d);
    std::cout << "to_string(42): " << s1 << std::endl;
    std::cout << "to_string(3.14): " << s2 << std::endl;

    // stoi/stol/stod: 字符串转数值
    std::string s3 = "123";
    std::string s4 = "3.14";
    int num = std::stoi(s3);
    double pi = std::stod(s4);
    std::cout << "stoi(\"123\"): " << num << std::endl;
    std::cout << "stod(\"3.14\"): " << pi << std::endl;

    std::cout << std::endl;
}

// --------------------------------------------
// 10. 使用注意事项
// --------------------------------------------
void test10_pitfalls() {
    std::cout << "=== 使用注意事项 ===" << std::endl;

    // ⚠️ 注意事项1: 空字符串与NULL不同
    std::string s1;       // 空字符串，合法
    // std::string s2 = NULL;  // 错误！不能将NULL赋给string
    std::cout << "空字符串长度: " << s1.length() << std::endl;

    // ⚠️ 注意事项2: c_str()返回的指针在string修改后会失效
    std::string s2 = "Hello";
    const char* p = s2.c_str();
    s2 += " World";  // 修改string后，p可能失效
    // std::cout << p << std::endl;  // 危险！可能输出错误内容
    std::cout << "修改后重新获取: " << s2.c_str() << std::endl;

    // ⚠️ 注意事项3: 使用 at() 而非 [] 进行安全的边界检查
    std::string s3 = "Hello";
    // std::cout << s3[10] << std::endl;  // 未定义行为！
    try {
        char c = s3.at(10);  // 抛出异常
    } catch (const std::out_of_range& e) {
        std::cout << "越界访问被捕获: " << e.what() << std::endl;
    }

    // ⚠️ 注意事项4: 短字符串优化(SSO)
    // 小字符串（通常15字节以内）可能存储在栈上，不需要堆分配
    std::string s4 = "short";  // 可能在栈上
    std::string s5(1000, 'x'); // 肯定在堆上
    std::cout << "短字符串地址: " << static_cast<const void*>(s4.c_str()) << std::endl;
    std::cout << "长字符串地址: " << static_cast<const void*>(s5.c_str()) << std::endl;

    // ⚠️ 注意事项5: string reserve 和 capacity
    std::string s6;
    std::cout << "初始容量: " << s6.capacity() << std::endl;
    s6.reserve(100);  // 预分配空间，减少重新分配
    std::cout << "reserve后容量: " << s6.capacity() << std::endl;

    // ⚠️ 注意事项6: 避免频繁的字符串拼接
    // 使用 reserve 减少重新分配
    std::string s7;
    s7.reserve(100);
    for (int i = 0; i < 10; ++i) {
        s7 += "a";
    }
    std::cout << "高效拼接: " << s7 << std::endl;

    // ⚠️ 注意事项7: 字符串视图(string_view)避免拷贝（C++17）
    // 当只需要读取字符串时，使用 std::string_view 避免拷贝

    std::cout << std::endl;
}

// --------------------------------------------
// 总结
// --------------------------------------------
/*
 * std::string 关键要点：
 *
 * 1. 构造方式多样：字面量、字符重复、拷贝、子串等
 * 2. 安全的动态数组：自动管理内存，避免缓冲区溢出
 * 3. 丰富的操作：拼接、比较、查找、替换等
 * 4. 与C字符串互操作：c_str()返回const char*
 * 5. 数值转换：to_string()和stoi/stod等函数
 *
 * 最佳实践：
 * - 优先使用 std::string 而非 C 风格字符串
 * - 使用 at() 进行边界检查，或确保索引合法
 * - 大量拼接时使用 reserve() 减少重新分配
 * - 只读场景考虑使用 std::string_view (C++17)
 * - 注意 c_str() 返回指针的生命周期
 */

int main() {
    test01_construction();
    test02_basic_operations();
    test03_element_access();
    test04_concatenation();
    test05_comparison();
    test06_find_and_substring();
    test07_modification();
    test08_iteration();
    test09_number_conversion();
    test10_pitfalls();

    return 0;
}
