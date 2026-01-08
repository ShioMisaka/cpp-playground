#include <iostream>
#include <vector>
#include "test_class/my_AVLTree.h"

// =====================================================
// AVL 树性质回顾
// =====================================================
// 1. 任何节点的左子树和右子树的高度差的绝对值不能超过 1
//    平衡因子 = 左子树高度 - 右子树高度
// 2. 平衡因子只可能是: -1, 0, 1
// 3. 当平衡因子超出范围时，需要通过旋转来恢复平衡
// =====================================================

void printSeparator(const std::string& title) {
    std::cout << "\n========== " << title << " ==========\n" << std::endl;
}

// =====================================================
// 测试01: 基础插入 - 理解 AVL 树平衡
// =====================================================
void test01_basic_insert()
{
    printSeparator("测试01: 基础插入");

    AVLTree<int> tree;

    std::vector<int> keys = {50, 30, 70, 20, 40, 60, 80};

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
// 测试02: RR 旋转（左单旋）
// =====================================================
void test02_rr_rotation()
{
    printSeparator("测试02: RR 旋转（左单旋）");

    std::cout << "插入序列: 10 -> 20 -> 30\n" << std::endl;
    std::cout << "分析过程:" << std::endl;
    std::cout << "  1. 插入 10: 平衡因子 0, 正常" << std::endl;
    std::cout << "  2. 插入 20: 节点10平衡因子 -1, 正常" << std::endl;
    std::cout << "  3. 插入 30: 节点10平衡因子 -2, 失衡!" << std::endl;
    std::cout << "     触发 RR 型 -> 左旋 10, 20 成为新根\n" << std::endl;

    AVLTree<int> tree;
    std::vector<int> keys = {10, 20, 30};

    for (int key : keys) {
        tree.insert(key);
        std::cout << "插入 " << key << ": ";
        tree.printInOrder();
    }

    std::cout << "\n最终树结构 (根为 20):" << std::endl;
    std::cout << "    20" << std::endl;
    std::cout << "   /  \\" << std::endl;
    std::cout << "  10  30" << std::endl;
}

// =====================================================
// 测试03: LL 旋转（右单旋）
// =====================================================
void test03_ll_rotation()
{
    printSeparator("测试03: LL 旋转（右单旋）");

    std::cout << "插入序列: 30 -> 20 -> 10\n" << std::endl;
    std::cout << "分析过程:" << std::endl;
    std::cout << "  1. 插入 30: 平衡因子 0, 正常" << std::endl;
    std::cout << "  2. 插入 20: 节点30平衡因子 1, 正常" << std::endl;
    std::cout << "  3. 插入 10: 节点30平衡因子 2, 失衡!" << std::endl;
    std::cout << "     触发 LL 型 -> 右旋 30, 20 成为新根\n" << std::endl;

    AVLTree<int> tree;
    std::vector<int> keys = {30, 20, 10};

    for (int key : keys) {
        tree.insert(key);
        std::cout << "插入 " << key << ": ";
        tree.printInOrder();
    }

    std::cout << "\n最终树结构 (根为 20):" << std::endl;
    std::cout << "    20" << std::endl;
    std::cout << "   /  \\" << std::endl;
    std::cout << "  10  30" << std::endl;
}

// =====================================================
// 测试04: LR 旋转（先左后右）
// =====================================================
void test04_lr_rotation()
{
    printSeparator("测试04: LR 旋转（先左后右）");

    std::cout << "插入序列: 30 -> 10 -> 20\n" << std::endl;
    std::cout << "分析过程:" << std::endl;
    std::cout << "  1. 插入 30: 平衡因子 0, 正常" << std::endl;
    std::cout << "  2. 插入 10: 节点30平衡因子 1, 正常" << std::endl;
    std::cout << "  3. 插入 20: 节点30平衡因子 2, 失衡!" << std::endl;
    std::cout << "     这是 LR 型 (左孩子的右子树):" << std::endl;
    std::cout << "     步骤1: 左旋节点 10, 使 20 成为左孩子" << std::endl;
    std::cout << "     步骤2: 右旋节点 30, 使 20 成为根\n" << std::endl;

    AVLTree<int> tree;
    std::vector<int> keys = {30, 10, 20};

    for (int key : keys) {
        tree.insert(key);
        std::cout << "插入 " << key << ": ";
        tree.printInOrder();
    }

    std::cout << "\n最终树结构 (根为 20):" << std::endl;
    std::cout << "    20" << std::endl;
    std::cout << "   /  \\" << std::endl;
    std::cout << "  10  30" << std::endl;
}

// =====================================================
// 测试05: RL 旋转（先右后左）
// =====================================================
void test05_rl_rotation()
{
    printSeparator("测试05: RL 旋转（先右后左）");

    std::cout << "插入序列: 10 -> 30 -> 20\n" << std::endl;
    std::cout << "分析过程:" << std::endl;
    std::cout << "  1. 插入 10: 平衡因子 0, 正常" << std::endl;
    std::cout << "  2. 插入 30: 节点10平衡因子 -1, 正常" << std::endl;
    std::cout << "  3. 插入 20: 节点10平衡因子 -2, 失衡!" << std::endl;
    std::cout << "     这是 RL 型 (右孩子的左子树):" << std::endl;
    std::cout << "     步骤1: 右旋节点 30, 使 20 成为右孩子" << std::endl;
    std::cout << "     步骤2: 左旋节点 10, 使 20 成为根\n" << std::endl;

    AVLTree<int> tree;
    std::vector<int> keys = {10, 30, 20};

    for (int key : keys) {
        tree.insert(key);
        std::cout << "插入 " << key << ": ";
        tree.printInOrder();
    }

    std::cout << "\n最终树结构 (根为 20):" << std::endl;
    std::cout << "    20" << std::endl;
    std::cout << "   /  \\" << std::endl;
    std::cout << "  10  30" << std::endl;
}

// =====================================================
// 测试06: 综合插入 - 多次旋转
// =====================================================
void test06_complex_insert()
{
    printSeparator("测试06: 综合插入");

    AVLTree<int> tree;
    std::vector<int> keys = {41, 20, 65, 11, 29, 50, 26};

    std::cout << "插入序列: 41, 20, 65, 11, 29, 50, 26\n" << std::endl;
    std::cout << "说明: 这个序列会触发多次旋转操作\n" << std::endl;

    for (size_t i = 0; i < keys.size(); i++) {
        tree.insert(keys[i]);
        std::cout << "插入 " << keys[i] << ": ";
        tree.printInOrder();
    }

    std::cout << "\n树始终保持平衡, 高度约为 log2(n)" << std::endl;
}

// =====================================================
// 测试07: 删除操作
// =====================================================
void test07_delete()
{
    printSeparator("测试07: 删除操作");

    AVLTree<int> tree;
    std::vector<int> keys = {50, 30, 70, 20, 40, 60, 80};

    std::cout << "先构建树: ";
    for (int key : keys) {
        tree.insert(key);
        std::cout << key << " ";
    }
    std::cout << "\n" << std::endl;

    std::cout << "初始中序遍历: ";
    tree.printInOrder();

    // 删除叶子节点
    std::cout << "\n删除叶子节点 20:" << std::endl;
    tree.deleteNode(20);
    std::cout << "删除后: ";
    tree.printInOrder();

    // 删除有一个孩子的节点
    std::cout << "\n删除有一个孩子的节点 30:" << std::endl;
    tree.deleteNode(30);
    std::cout << "删除后: ";
    tree.printInOrder();

    // 删除有两个孩子的节点
    std::cout << "\n删除有两个孩子的节点 50 (根节点):" << std::endl;
    tree.deleteNode(50);
    std::cout << "删除后: ";
    tree.printInOrder();
}

// =====================================================
// 测试08: 搜索操作
// =====================================================
void test08_search()
{
    printSeparator("测试08: 搜索操作");

    AVLTree<int> tree;
    std::vector<int> keys = {50, 30, 70, 20, 40, 60, 80};

    for (int key : keys) {
        tree.insert(key);
    }

    std::cout << "树中已插入: ";
    for (int key : keys) {
        std::cout << key << " ";
    }
    std::cout << "\n" << std::endl;

    std::vector<int> searchKeys = {20, 50, 80, 100};
    for (int key : searchKeys) {
        std::cout << "搜索 " << key << ": "
                  << (tree.search(key) ? "找到" : "未找到") << std::endl;
    }
}

// =====================================================
// 测试09: 边界情况
// =====================================================
void test09_edge_cases()
{
    printSeparator("测试09: 边界情况");

    AVLTree<int> tree;

    std::cout << "1. 空树搜索:" << std::endl;
    std::cout << "   搜索 100: " << (tree.search(100) ? "找到" : "未找到") << std::endl;
    std::cout << std::endl;

    std::cout << "2. 只插入一个节点:" << std::endl;
    tree.insert(50);
    std::cout << "   插入 50: ";
    tree.printInOrder();

    std::cout << "\n3. 删除唯一节点:" << std::endl;
    tree.deleteNode(50);
    std::cout << "   删除后树为空" << std::endl;

    std::cout << "\n4. 顺序插入 (1,2,3,4,5) - 触发多次旋转:" << std::endl;
    for (int i = 1; i <= 5; i++) {
        tree.insert(i);
    }
    std::cout << "   中序遍历: ";
    tree.printInOrder();
}

// =====================================================
// 测试10: 大规模数据
// =====================================================
void test10_large_scale()
{
    printSeparator("测试10: 大规模数据");

    AVLTree<int> tree;

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

    test01_basic_insert();      // 基础插入
    test02_rr_rotation();       // RR 左旋
    test03_ll_rotation();       // LL 右旋
    test04_lr_rotation();       // LR 先左后右
    test05_rl_rotation();       // RL 先右后左
    test06_complex_insert();    // 复杂场景
    test07_delete();            // 删除操作
    test08_search();            // 搜索操作
    test09_edge_cases();        // 边界情况
    test10_large_scale();       // 大规模数据

    printSeparator("所有测试完成");
    std::cout << "总结: AVL 树通过四种旋转操作保持平衡" << std::endl;
    std::cout << "      LL: 右单旋 | RR: 左单旋" << std::endl;
    std::cout << "      LR: 先左后右 | RL: 先右后左" << std::endl;

    return 0;
}

/*
运行结果示例:

========== 测试01: 基础插入 ==========

依次插入: 50
插入 50 后的中序遍历: 50
30
插入 30 后的中序遍历: 30 50
70
插入 70 后的中序遍历: 30 50 70
...

========== 测试02: RR 旋转（左单旋） ==========

插入序列: 10 -> 20 -> 30

分析过程:
  1. 插入 10: 平衡因子 0, 正常
  2. 插入 20: 节点10平衡因子 -1, 正常
  3. 插入 30: 节点10平衡因子 -2, 失衡!
     触发 RR 型 -> 左旋 10, 20 成为新根

插入 10: 10
插入 20: 10 20
插入 30: 10 20 30

...
*/
