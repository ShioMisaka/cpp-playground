#include <iostream>

/*
	VAL树
	任何节点的左子树和右子树的高度差的绝对值不能超过1，平衡因子 = 左子树高度 - 右子树高度
	LL、RR、LR、RL

*/
#include "test_class/my_AVLTree.h"

void demonstrateRotations() 
{
	 std::cout << "--- 演示AVL树旋转 ---" << std::endl;

    // 1. 演示 RR (左单旋)
    AVLTree<int> rr_tree;
    std::cout << "\n插入 10, 20, 30 (触发RR旋转):" << std::endl;
    rr_tree.insert(10);
    rr_tree.insert(20);
    rr_tree.insert(30); // 节点10失衡, 平衡因子-2
    std::cout << "中序遍历: ";
    rr_tree.printInOrder(); // 应该输出 10 20 30

    // 2. 演示 LL (右单旋)
    AVLTree<int> ll_tree;
    std::cout << "\n插入 30, 20, 10 (触发LL旋转):" << std::endl;
    ll_tree.insert(30);
    ll_tree.insert(20);
    ll_tree.insert(10); // 节点30失衡, 平衡因子+2
    std::cout << "中序遍历: ";
    ll_tree.printInOrder(); // 应该输出 10 20 30

    // 3. 演示 LR (先左后右)
    AVLTree<int> lr_tree;
    std::cout << "\n插入 30, 10, 20 (触发LR旋转):" << std::endl;
    lr_tree.insert(30);
    lr_tree.insert(10);
    lr_tree.insert(20); // 节点30失衡, 平衡因子+2
    std::cout << "中序遍历: ";
    lr_tree.printInOrder(); // 应该输出 10 20 30

    // 4. 演示 RL (先右后左)
    AVLTree<int> rl_tree;
    std::cout << "\n插入 10, 30, 20 (触发RL旋转):" << std::endl;
    rl_tree.insert(10);
    rl_tree.insert(30);
    rl_tree.insert(20); // 节点10失衡, 平衡因子-2
    std::cout << "中序遍历: ";
    rl_tree.printInOrder(); // 应该输出 10 20 30
}


void val_tree_test()
{
	demonstrateRotations();

    std::cout << "\n--- 综合测试 ---" << std::endl;
    AVLTree<int> tree;
    std::vector<int> keys = {9, 5, 10, 0, 6, 11, -1, 1, 2};

    std::cout << "依次插入: ";
    for (int key : keys) {
        std::cout << key << " ";
        tree.insert(key);
    }
    std::cout << std::endl;

    std::cout << "中序遍历 (排序结果): ";
    tree.printInOrder();

    std::cout << "\n删除节点 10:" << std::endl;
    tree.deleteNode(10);
    std::cout << "中序遍历: ";
    tree.printInOrder();

    std::cout << "\n删除节点 1:" << std::endl;
    tree.deleteNode(1);
    std::cout << "中序遍历: ";
    tree.printInOrder();
    
    std::cout << "\n搜索 5: " << (tree.search(5) ? "找到" : "未找到") << std::endl;
    std::cout << "搜索 100: " << (tree.search(100) ? "找到" : "未找到") << std::endl;
}

/*
--- 演示AVL树旋转 ---

插入 10, 20, 30 (触发RR旋转):
中序遍历: 10 20 30 

插入 30, 20, 10 (触发LL旋转):
中序遍历: 10 20 30 

插入 30, 10, 20 (触发LR旋转):
中序遍历: 10 20 30 

插入 10, 30, 20 (触发RL旋转):
中序遍历: 10 20 30 

--- 综合测试 ---
依次插入: 9 5 10 0 6 11 -1 1 2 
中序遍历 (排序结果): -1 0 1 2 5 6 9 10 11 

删除节点 10:
中序遍历: -1 0 1 2 5 6 9 11 

删除节点 1:
中序遍历: -1 0 2 5 6 9 11 

搜索 5: 找到
搜索 100: 未找到
*/

/*
	红黑树性质
	(1) 每个节点要么是红色要么是黑色，根节点必须是黑色;
	(2) 任何相邻节点(上下关系)不能同为红色;
	(3) 任意叶子节点到根节点，所经过的黑色节点相同————黑高相同。
*/

#include "test_class/RBTree.h"

void rb_tree_test()
{
    {
        RBTree<int> tree;

        std::cout << "依次插入: 11, 2, 14, 1, 7, 15, 5, 8, 4\n" << std::endl;

        tree.insert(11);
        std::cout << "插入 11: "; tree.printInOrder();
        tree.insert(2);
        std::cout << "插入 2:  "; tree.printInOrder();
        tree.insert(14);
        std::cout << "插入 14: "; tree.printInOrder();
        tree.insert(1);
        std::cout << "插入 1:  "; tree.printInOrder();
        tree.insert(7);
        std::cout << "插入 7:  "; tree.printInOrder();
        tree.insert(15);
        std::cout << "插入 15: "; tree.printInOrder();
        tree.insert(5);
        std::cout << "插入 5:  "; tree.printInOrder();
        tree.insert(8);
        std::cout << "插入 8:  "; tree.printInOrder();
        tree.insert(4);
        std::cout << "插入 4:  "; tree.printInOrder();
        
        std::cout << "\n最终的中序遍历结果 (B代表黑色, R代表红色):" << std::endl;
        tree.printInOrder();
    }

    {
        RBTree<int> tree;
        std::vector<int> keys = {10, 20, 30, 40, 50, 25, 60, 75};

        std::cout << "--- 插入节点 ---" << std::endl;
        for (int key : keys) {
            tree.insert(key);
            std::cout << "插入 " << key << ": ";
            tree.printInOrder();
        }

        std::cout << "\n--- 删除节点 ---" << std::endl;
        
        // 删除一个有两个孩子的黑色节点 (可能触发复杂修复)
        int key_to_delete = 30;
        std::cout << "\n删除 " << key_to_delete << ":" << std::endl;
        tree.deleteNode(key_to_delete);
        tree.printInOrder();

        // 删除一个红色叶子节点 (最简单情况)
        key_to_delete = 25;
        std::cout << "\n删除 " << key_to_delete << ":" << std::endl;
        tree.deleteNode(key_to_delete);
        tree.printInOrder();

        // 删除根节点
        key_to_delete = 40;
        std::cout << "\n删除 " << key_to_delete << ":" << std::endl;
        tree.deleteNode(key_to_delete);
        tree.printInOrder();
        
        // 删除一个不存在的节点
        key_to_delete = 99;
        std::cout << "\n删除 " << key_to_delete << ":" << std::endl;
        tree.deleteNode(key_to_delete);
        tree.printInOrder();
    }
}

int main()
{

    rb_tree_test();
    return 0;
}
