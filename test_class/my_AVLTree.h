#pragma once

#include <iostream>
#include <algorithm>
#include <vector>

template <typename T>
class AVLTree
{
private:
    // 1. AVL 树节点结构
    struct AVLNode {
        T key;
        AVLNode *left;
        AVLNode *right;
        int height;

        // 构造函数
        AVLNode(T k) : key(k), left(nullptr), right(nullptr), height(1) {}
    };

    AVLNode *root_;

        // --- 私有辅助函数 ---

    // 获取节点高度 (处理空节点)
    int height(AVLNode *node){
        if (node == nullptr) return 0;
        return node->height;
    }
    // 更新节点高度
    void updateHeight(AVLNode *node) {
        if (node != nullptr)
            node->height = 1 + std::max(height(node->left), height(node->right));
    }
    // 计算节点的平衡因子
    int getBalance(AVLNode *node) {
        if (node == nullptr)
            return 0;
        return height(node->left) - height(node->right);
    }
    // 右单旋转 (处理 LL 失衡)
    AVLNode *rightRotate(AVLNode *y) {
        AVLNode *x = y->left;
        AVLNode *temp = x->right;

        x->right = y;
        y->left = temp;

        updateHeight(y);
        updateHeight(x);

        return x;
    }
    // 左单旋转 (处理 RR 失衡)
    AVLNode *leftRotate(AVLNode *x)
    {
        AVLNode *y = x->right;
        AVLNode *temp = y->left;

        y->left = x;
        x->right = temp;

        updateHeight(x);
        updateHeight(y);

        return y;
    }
    // 查找子树的最小节点 (用于删除)
    AVLNode *findMin(AVLNode *node) {
        AVLNode *cur = node;
        while (cur->left != nullptr)
            cur = cur->left;
        return cur;
    }
    // 递归插入节点
    AVLNode *insert(AVLNode *node, T key) {
        if (node == nullptr)
            return new AVLNode(key);

        if (key < node->key)
            node->left = insert(node->left, key);
        else if (key > node->key)
            node->right = insert(node->right, key);
        else return node;

        updateHeight(node);

        int blence = getBalance(node);

        if (blence > 1 && key < node->left->key) return rightRotate(node);
        if (blence < -1 && key > node->right->key) return leftRotate(node);
        if (blence > 1 && key > node->left->key) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        if (blence < -1 && key < node->right->key) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }
    // 递归删除节点
    AVLNode *deleteNode(AVLNode *root, T key) {
        if (root == nullptr)
            return root;

        if (key < root->key) {
            root->left = deleteNode(root->left, key);
        } else if (key > root->key) {
            root->right = deleteNode(root->right, key);
        } else {
            if (root->left == nullptr || root->right == nullptr) {
                AVLNode *temp = root->left ? root->left : root->right;
                if (temp == nullptr) {
                    temp = root;
                    root = nullptr;
                } else 
                    *root = *temp;
                delete temp;
            } else {
                AVLNode *temp = findMin(root->right);
                root->key = temp->key;
                root->right = deleteNode(root->right, temp->key);
            }
        }

        if (root == nullptr) return root;

        updateHeight(root);

        int blence = getBalance(root);

        if (blence > 1 && getBalance(root->left) >= 0) return rightRotate(root);
        if (blence < -1 && getBalance(root->right) <= 0) return leftRotate(root);
        if (blence > 1 && getBalance(root->left) < 0) {
            root->right = leftRotate(root->right);
            return rightRotate(root);
        }
        if (blence < -1 && getBalance(root->right) > 0) {
            root->left = rightRotate(root->left);
            return leftRotate(root);
        }

        return root;
    }
    // 递归搜索
    bool search(AVLNode *node, T key) {
        if(node == nullptr) return false;
        if (key < node->key) return search(node->left, key);
        else if (key > node->key) return search(node->right, key);
        else return true;
    }
    // 中序遍历 (用于打印排序结果)
    void inOrder(AVLNode *root) {
        if (root != nullptr) {
            inOrder(root->left);
            std::cout << root->key << " ";
            inOrder(root->right);
        }
    }
    // 递归销毁树
    void destroyTree(AVLNode *node) {
        if (node) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

public:
    // --- 公有接口 ---

    AVLTree() : root_(nullptr) {}

    // 析构函数，释放所有节点
    ~AVLTree() {
        destroyTree(root_);
    }

    void insert(T key) {
        root_ = insert(root_, key);
    }

    void deleteNode(T key) {
        root_ = deleteNode(root_, key);
    }
    
    bool search(T key) {
        return search(root_, key);
    }

    void printInOrder() {
        inOrder(root_);
        std::cout << std::endl;
    }
};