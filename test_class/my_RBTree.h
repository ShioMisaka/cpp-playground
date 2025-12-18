#pragma once


template <typename T>
class RBTree
{
private:
    enum class Color {RED, BLACK};

    struct RBNode {
        T key;
        Color color;
        RBNode *left, *right, *parent;

        RBNode(T key) : key(key), color(Color::RED), left(nullptr), right(nullptr), parent(nullptr) {}
    }

    
private:
    RBNode* root_;
    RBNode* NIL_; // 哨兵节点，代表所有叶子节点

    // 左旋
    void leftRotate(RBNode* x) {
        RBNode *y = x->right;
        RBNode 
    }

    // 右旋
    void rightRotate(RBNode *x);

    // 插入后的修复操作
    void insertFixup(RBNode *k);

    // 递归中序遍历
    void inOrderHelper(RBNode *node);

    // 递归销毁
    void destroyTree(RBNode *node);
};