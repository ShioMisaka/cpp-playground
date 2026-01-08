#include <iostream>
#include <vector>
#include "test_class/RBTree.h"

// =====================================================
// 红黑树性质回顾
// =====================================================
// 1. 每个节点不是红色就是黑色
// 2. 根节点是黑色
// 3. 所有叶子节点（NIL哨兵）是黑色
// 4. 红色节点的两个子节点必须是黑色（不能有连续红节点）
// 5. 从任意节点到其每个叶子的所有路径都包含相同数目的黑色节点
// =====================================================

void printSeparator(const std::string& title) {
    std::cout << "\n========== " << title << " ==========\n" << std::endl;
}

// =====================================================
// 测试01: 基础插入 - 理解红黑树性质
// =====================================================
void test01_basic_insert()
{
    printSeparator("测试01: 基础插入");

    RBTree<int> tree;

    // 逐个插入并观察结果
    std::vector<int> keys = {10, 5, 15, 3, 7, 12, 18};

    std::cout << "依次插入: ";
    for (int key : keys) {
        std::cout << key << " ";
        tree.insert(key);
        std::cout << "\n插入 " << key << " 后的中序遍历: ";
        tree.printInOrder();
    }

    std::cout << "\n最终结果: 所有节点按 BST 性质有序排列" << std::endl;
    std::cout << "中序遍历: ";
    tree.printInOrder();
}

// =====================================================
// 测试02: 修复情况演示 - 叔叔是红色
// =====================================================
void test02_uncle_red()
{
    printSeparator("测试02: 叔叔是红色的情况");

    RBTree<int> tree;

    // 插入顺序触发: 叔叔红色的情况
    // 插入 10, 5, 15 后，树是平衡的
    // 插入 3 后，3 的父节点 5 是红色，叔叔 15 也是红色
    std::cout << "插入序列: 10 -> 5 -> 15 -> 3\n" << std::endl;
    std::cout << "分析: 插入 3 时，节点 5(红) 的兄弟 15(红) 是叔叔" << std::endl;
    std::cout << "处理: 父亲和叔叔变黑，祖父变红，继续向上检查\n" << std::endl;

    std::vector<int> keys = {10, 5, 15, 3};
    for (int key : keys) {
        tree.insert(key);
        std::cout << "插入 " << key << ": ";
        tree.printInOrder();
    }
}

// =====================================================
// 测试03: 修复情况演示 - 叔叔是黑色（LL/LR型）
// =====================================================
void test03_uncle_black_left()
{
    printSeparator("测试03: 叔叔是黑色 - 左侧情况");

    // LL 情况
    {
        std::cout << "[LL型] 插入序列: 30 -> 20 -> 10\n" << std::endl;
        std::cout << "分析: 连续左倾，触发右旋" << std::endl;

        RBTree<int> tree;
        std::vector<int> keys = {30, 20, 10};
        for (int key : keys) {
            tree.insert(key);
            std::cout << "插入 " << key << ": ";
            tree.printInOrder();
        }
        std::cout << std::endl;
    }

    // LR 情况
    {
        std::cout << "[LR型] 插入序列: 30 -> 10 -> 20\n" << std::endl;
        std::cout << "分析: 先左旋父节点，转为 LL 型，再右旋祖父" << std::endl;

        RBTree<int> tree;
        std::vector<int> keys = {30, 10, 20};
        for (int key : keys) {
            tree.insert(key);
            std::cout << "插入 " << key << ": ";
            tree.printInOrder();
        }
    }
}

// =====================================================
// 测试04: 修复情况演示 - 叔叔是黑色（RR/RL型）
// =====================================================
void test04_uncle_black_right()
{
    printSeparator("测试04: 叔叔是黑色 - 右侧情况");

    // RR 情况
    {
        std::cout << "[RR型] 插入序列: 10 -> 20 -> 30\n" << std::endl;
        std::cout << "分析: 连续右倾，触发左旋" << std::endl;

        RBTree<int> tree;
        std::vector<int> keys = {10, 20, 30};
        for (int key : keys) {
            tree.insert(key);
            std::cout << "插入 " << key << ": ";
            tree.printInOrder();
        }
        std::cout << std::endl;
    }

    // RL 情况
    {
        std::cout << "[RL型] 插入序列: 10 -> 30 -> 20\n" << std::endl;
        std::cout << "分析: 先右旋父节点，转为 RR 型，再左旋祖父" << std::endl;

        RBTree<int> tree;
        std::vector<int> keys = {10, 30, 20};
        for (int key : keys) {
            tree.insert(key);
            std::cout << "插入 " << key << ": ";
            tree.printInOrder();
        }
    }
}

// =====================================================
// 测试05: 复杂插入 - 多次修复
// =====================================================
void test05_complex_insert()
{
    printSeparator("测试05: 复杂插入场景");

    RBTree<int> tree;
    std::vector<int> keys = {11, 2, 14, 1, 7, 15, 5, 8, 4};

    std::cout << "插入序列: 11, 2, 14, 1, 7, 15, 5, 8, 4\n" << std::endl;
    std::cout << "说明: 这个序列会触发多种修复情况\n" << std::endl;

    for (int key : keys) {
        tree.insert(key);
        std::cout << "插入 " << key << ": ";
        tree.printInOrder();
    }
}

// =====================================================
// 测试06: 删除操作
// =====================================================
void test06_delete()
{
    printSeparator("测试06: 删除操作");

    RBTree<int> tree;
    std::vector<int> keys = {10, 20, 30, 40, 50, 25, 60, 75};

    std::cout << "先构建树: ";
    for (int key : keys) {
        tree.insert(key);
        std::cout << key << " ";
    }
    std::cout << "\n" << std::endl;

    std::cout << "初始中序遍历: ";
    tree.printInOrder();

    // 删除各种情况的节点
    std::vector<int> toDelete = {30, 25, 40, 99};
    for (int key : toDelete) {
        std::cout << "\n删除节点 " << key << ":" << std::endl;
        tree.deleteNode(key);
        std::cout << "删除后中序遍历: ";
        tree.printInOrder();
    }
}

// =====================================================
// 测试07: 边界情况
// =====================================================
void test07_edge_cases()
{
    printSeparator("测试07: 边界情况");

    RBTree<int> tree;

    std::cout << "1. 空树删除:" << std::endl;
    tree.deleteNode(100);
    std::cout << std::endl;

    std::cout << "2. 只插入一个节点:" << std::endl;
    tree.insert(50);
    std::cout << "插入 50: ";
    tree.printInOrder();

    std::cout << "\n3. 删除唯一节点:" << std::endl;
    tree.deleteNode(50);

    std::cout << "\n4. 重复值处理 (后插入的会被忽略):" << std::endl;
    tree.insert(10);
    tree.insert(10);
    std::cout << "插入两次 10: ";
    tree.printInOrder();
}

// =====================================================
// 测试08: 大规模数据
// =====================================================
void test08_large_scale()
{
    printSeparator("测试08: 大规模数据");

    RBTree<int> tree;

    std::cout << "插入 1-20 的连续数字..." << std::endl;
    for (int i = 1; i <= 20; i++) {
        tree.insert(i);
    }
    std::cout << "中序遍历结果: ";
    tree.printInOrder();

    std::cout << "\n验证 BST 性质: 应该输出 1 2 3 ... 20" << std::endl;

    std::cout << "\n删除所有奇数..." << std::endl;
    for (int i = 1; i <= 20; i += 2) {
        tree.deleteNode(i);
    }
    std::cout << "删除后中序遍历: ";
    tree.printInOrder();
}

// =====================================================
// 主函数
// =====================================================
int main()
{
    // 按顺序运行测试，从简单到复杂

    test01_basic_insert();       // 基础插入
    test02_uncle_red();          // 叔叔红色情况
    test03_uncle_black_left();   // 叔叔黑色（左）
    test04_uncle_black_right();  // 叔叔黑色（右）
    test05_complex_insert();     // 复杂场景
    test06_delete();             // 删除操作
    test07_edge_cases();         // 边界情况
    test08_large_scale();        // 大规模数据

    printSeparator("所有测试完成");
    std::cout << "说明: R=红色节点, B=黑色节点" << std::endl;
    std::cout << "      中序遍历验证了 BST 的有序性" << std::endl;

    return 0;
}

/*
运行结果示例:

========== 测试01: 基础插入 ==========

依次插入: 10
插入 10 后的中序遍历: 10(B)
插入 5 后的中序遍历: 5(R) 10(B)
插入 15 后的中序遍历: 5(B) 10(B) 15(B)
...

========== 测试02: 叔叔是红色的情况 ==========

插入序列: 10 -> 5 -> 15 -> 3

分析: 插入 3 时，节点 5(红) 的兄弟 15(红) 是叔叔
处理: 父亲和叔叔变黑，祖父变红，继续向上检查

插入 10: 10(B)
插入 5: 5(R) 10(B)
插入 15: 5(B) 10(B) 15(B)
插入 3: 3(R) 5(B) 10(R) 15(B)

...
*/
