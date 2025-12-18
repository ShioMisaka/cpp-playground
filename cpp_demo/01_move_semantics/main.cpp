#include <iostream>
#include <utility>
#include <type_traits>

#include <mystl/utility.h>
#include <test_class/Entity.h>
/*
+--------------------------------------------------+
|                   Expression                     |
|                                                  |
|  +---------------------+   +-------------------+ |
|  |     glvalue         |   |     prvalue       | |
|  | (has identity)      |   | (no identity)     | |
|  |                     |   |                   | |
|  |  +------------+     |   |                   | |
|  |  |  lvalue    |     |   |                   | |
|  |  | (can be    |     |   |                   | |
|  |  | copied)    |     |   |                   | |
|  |  +------------+     |   |                   | |
|  |        ^            |   |                   | |
|  |        |            |   |                   | |
|  |  +------------+     |   |                   | |
|  |  |  xvalue    |     |   |                   | |
|  |  | (can be    |     |   |                   | |
|  |  | moved from)|     |   |                   | |
|  |  +------------+     |   |                   | |
|  +---------------------+   +-------------------+ |
|            \                     /               |
|             \                   /                |
|              +-----------------+                 |
|              |     rvalue      |                 |
|              | (can be moved)  |                 |
|              +-----------------+                 |
+--------------------------------------------------+
*/

/*
lvalue  左值
rvalue  右值
xvalue  将亡值
glvalue 泛左值
prvalue 纯右值
*/

// 1、左值引用和右值引用的区别
void test01()
{
    // （1）左值引用
    Entity i(1);
    Entity& lvalue = i; 

    // （2）右值引用
    Entity&& rvalue = Entity(2);
    // 右值引用无法接收一个左值
    // Entity&& ravlue0 = i; // × 错误

    // 现在 rvalue 已经有自己的一个名字了，他已经成为了一个左值，不能再被右值引用，除非使用 std::move 进行显示转换
    // int&& rvalue2 = rvalue; // × 错误
    Entity&& rvalue2 = std::move(rvalue); // √ 正确

    // std::move 的作用是将一个左值表达式无条件的转换为一个将亡值（xvalue）
    // 右值引用可以接受 右值（rvalue） 和 将亡值（xvalue）
    // 通过左值引用和右值引用进行区分，可以正确的调用 拷贝构造 和 移动构造
    std::cout << "--- end ---" << std::endl;
}


// 2、万能引用和引用折叠
template<typename T>
void Func(T&& t) {
    using RawT = std::remove_reference_t<T>;
    std::cout << std::boolalpha;
    std::cout << "is_lvalue_reference<T>: " << std::is_lvalue_reference_v<T> << "\n";
    std::cout << "is_rvalue_reference<T>: " << std::is_rvalue_reference_v<T> << "\n";
    std::cout << "Raw type is same as Entity: " << std::is_same_v<RawT, T> << "\n";

	
    std::cout << mystl::type_name<decltype(t)>() << "\n"; // int
}


void test02()
{
    //（1）万能引用
    // T&& 不完全是我上面所说的 右值引用 ，在模板参数推导的上下文中，T&& 有一个特殊的名字——万能引用（Scott Meyers提出的术语），现在更被官方地称为转发引用（Forwarding Reference）
    // 右值引用不能接收左值，而万能引用可以根据传入类型自动推导

    Entity x(1);
    Func(x);   // 传入左值: T被推导为 Entity&, t 的类型是 Entity& &&
    Func(Entity(2)); // 传入右值: T被推导为 Entity,  t 的类型是 Entity&&

    // （2）引用折叠
    /*
    C++语法不允许直接声明“引用的引用”，比如 int& & x; 是非法的。但在模板实例化过程中，这种结构会间接出现。引用折叠规则定义了如何将这种“引用的引用”简化为单个引用
    A& &   -> A&  （左值引用的左值引用 -> 左值引用）
    A& &&  -> A&  （左值引用的右值引用 -> 左值引用）
    A&& &  -> A&  （右值引用的左值引用 -> 左值引用）
    A&& && -> A&& （右值引用的右值引用 -> 右值引用）
    */
}

// 3、完美转发
// 完美转发例子
template <typename T>
constexpr T&& forward(typename std::remove_reference<T>::type& arg) noexcept {
    return static_cast<T&&>(arg);
}

template <typename T>
constexpr T&& forward(typename std::remove_reference<T>::type&& arg) noexcept {
    return static_cast<T &&>(arg);
}

/*
当传入左值时:
T 被推导为 MyType&。
T&& 经过引用折叠后是 MyType& && -> MyType&。
static_cast<MyType&>(arg): 将一个左值转换为左值引用，什么也不做，返回的依然是左值。
当传入右值时:
T 被推导为 MyType。
T&& 就是 MyType&&。
static_cast<MyType&&>(arg): 将一个左值 arg 强制转换为右值引用。这正是 std::move 的行为。
*/


template<typename T, typename... Args>
T* factory(Args&&... args) {
    return new T(mystl::forward<Args>(args)...);
}

template<typename T, typename... Args>
T* factory_noperfect(Args&&... args) {
    return new T(args...);
}

void test03()
{
	// 没有完美转发的话，当传入右值，都会被万能引用转为一个左值进行转递，导致没有调用移动语义
	std::cout << " ***左值转发*** " << std::endl;
	std::cout << "--- construct pr2 ---" << std::endl;
	Entity *pr2 = factory_noperfect<Entity>(Entity(2)); // 传入一个右值
	delete pr2;
	
	Entity er(4);
	std::cout << "--- construct pr5 ---" << std::endl;
	Entity *pr5 = factory_noperfect<Entity>(std::move(er)); // 将左值转为一个将亡值
	delete pr5;
	
	std::cout << "--- end ---" << std::endl;

	// 结论：std::forward<T>(arg) 做的事情是：
    // 如果原始传入的参数是右值，它就将 arg 转换为右值；
    // 如果原始传入的参数是左值，它就什么都不做，让 arg 保持为左值。它完美地“转发”了参数的值类别。

    std::cout << " ***完美转发*** " << std::endl;
    // 传入右值
    std::cout << "--- construct p1 ---" << std::endl;
    Entity *p1 = factory<Entity>(1);
    delete p1;
    std::cout << "--- construct p2 ---" << std::endl;
    Entity *p2 = factory<Entity>(Entity(2));
    delete p2;
    
    Entity e(4);

    // 传入左值 （泛左值）
    std::cout << "--- construct p3 ---" << std::endl;
    int i = 3;
    Entity *p3 = factory<Entity>(i);
    delete p3;
    std::cout << "--- construct p4 ---" << std::endl;
    Entity *p4 = factory<Entity>(e);
    delete p4;
    std::cout << "--- construct p5 ---" << std::endl;
    Entity *p5 = factory<Entity>(std::move(e));
    delete p5;

    std::cout << "--- end ---" << std::endl;
}

// 关于 return std::move(a) 是否存在问题
Entity getEntity(){
    Entity a(1);
    return a;
}

#include "mystl/utility.h"

void test04()
{
    // Entity&& a = getEntity();
    // std::cout << a << '\n';

    // Entity b = mystl::move(a);
    // std::cout << b << '\n';

    int &&int1 = 1;

    std::string &&str = "sad";

    const std::string &str2 = "sad";
    std::string &&str3 = "asd";

    std::cout << str3 << std::endl;

    str3 += "000";

    std::cout << str3 << std::endl;

    std::string &str4 = str3;

    
}

int main()
{
    test04();

    return 0;
}
