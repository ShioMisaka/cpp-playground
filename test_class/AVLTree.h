#ifndef AVLTREE_H
#define AVLTREE_H

#include <iostream>
#include <algorithm> // 用于 std::max
#include <vector>

// 使用模板以支持不同数据类型
template <typename T>
class AVLTree {
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

    AVLNode *root;

    // --- 私有辅助函数 ---

    // 获取节点高度 (处理空节点)
    int height(AVLNode* node) {
        if (node == nullptr)
            return 0;
        return node->height;
    }

    // 更新节点高度
    void updateHeight(AVLNode* node) {
        if (node != nullptr) {
            node->height = 1 + std::max(height(node->left), height(node->right));
        }
    }

    // 计算节点的平衡因子
    int getBalance(AVLNode* node) {
        if (node == nullptr)
            return 0;
        return height(node->left) - height(node->right);
    }

    // 右单旋转 (处理 LL 失衡)
    AVLNode* rightRotate(AVLNode* y) {
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;

        // 执行旋转
        x->right = y;
        y->left = T2;

        // 更新高度 (必须先更新y，再更新x)
        updateHeight(y);
        updateHeight(x);
        
        // 返回新的根节点
        return x;
    }

    // 左单旋转 (处理 RR 失衡)
    AVLNode* leftRotate(AVLNode* x) {
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;

        // 执行旋转
        y->left = x;
        x->right = T2;

        // 更新高度 (必须先更新x，再更新y)
        updateHeight(x);
        updateHeight(y);

        // 返回新的根节点
        return y;
    }

    // 查找子树的最小节点 (用于删除)
    AVLNode* findMin(AVLNode* node) {
        AVLNode* current = node;
        while (current->left != nullptr)
            current = current->left;
        return current;
    }
    
    // 递归插入节点
    AVLNode* insert(AVLNode* node, T key) {
        // 1. 标准的BST插入
        if (node == nullptr)
            return new AVLNode(key);

        if (key < node->key)
            node->left = insert(node->left, key);
        else if (key > node->key)
            node->right = insert(node->right, key);
        else // 不允许插入重复键
            return node;

        // 2. 更新当前节点的高度
        updateHeight(node);

        // 3. 计算平衡因子
        int balance = getBalance(node);

        // 4. 检查是否失衡并执行旋转
        // LL Case
        if (balance > 1 && key < node->left->key)
            return rightRotate(node);

        // RR Case
        if (balance < -1 && key > node->right->key)
            return leftRotate(node);

        // LR Case
        if (balance > 1 && key > node->left->key) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // RL Case
        if (balance < -1 && key < node->right->key) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        
        // 返回（可能已旋转的）节点指针
        return node;
    }

    // 递归删除节点
    AVLNode* deleteNode(AVLNode* root, T key) {
        // 1. 标准的BST删除
        if (root == nullptr)
            return root;

        if (key < root->key)
            root->left = deleteNode(root->left, key);
        else if (key > root->key)
            root->right = deleteNode(root->right, key);
        else {
            // 找到要删除的节点
            if ((root->left == nullptr) || (root->right == nullptr)) {
                AVLNode* temp = root->left ? root->left : root->right;
                if (temp == nullptr) { // 没有子节点
                    temp = root;
                    root = nullptr;
                } else { // 一个子节点
                    *root = *temp; // 拷贝内容
                }
                delete temp;
            } else {
                // 有两个子节点的节点
                AVLNode* temp = findMin(root->right); // 找到中序后继
                root->key = temp->key; // 拷贝后继的内容到此节点
                root->right = deleteNode(root->right, temp->key); // 删除中序后继
            }
        }

        if (root == nullptr)
            return root;

        // 2. 更新高度
        updateHeight(root);
        
        // 3. 获取平衡因子
        int balance = getBalance(root);

        // 4. 平衡树
        // LL Case
        if (balance > 1 && getBalance(root->left) >= 0)
            return rightRotate(root);

        // LR Case
        if (balance > 1 && getBalance(root->left) < 0) {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }

        // RR Case
        if (balance < -1 && getBalance(root->right) <= 0)
            return leftRotate(root);

        // RL Case
        if (balance < -1 && getBalance(root->right) > 0) {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }

        return root;
    }
    
    // 递归搜索
    bool search(AVLNode* node, T key) {
        if (node == nullptr) {
            return false;
        }
        if (key == node->key) {
            return true;
        }
        if (key < node->key) {
            return search(node->left, key);
        }
        return search(node->right, key);
    }

    // 中序遍历 (用于打印排序结果)
    void inOrder(AVLNode* root) {
        if (root != nullptr) {
            inOrder(root->left);
            std::cout << root->key << " ";
            inOrder(root->right);
        }
    }

    // 递归销毁树
    void destroyTree(AVLNode* node) {
        if (node) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }


public:
    // --- 公有接口 ---

    AVLTree() : root(nullptr) {}

    // 析构函数，释放所有节点
    ~AVLTree() {
        destroyTree(root);
    }

    void insert(T key) {
        root = insert(root, key);
    }

    void deleteNode(T key) {
        root = deleteNode(root, key);
    }
    
    bool search(T key) {
        return search(root, key);
    }

    void printInOrder() {
        inOrder(root);
        std::cout << std::endl;
    }
};

#endif // AVLTREE_H