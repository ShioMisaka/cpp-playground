#ifndef RBTREE_H
#define RBTREE_H

#include <iostream>

// 颜色枚举
enum Color { RED, BLACK };

template <typename T>
struct Node {
    T key;
    Color color;
    Node<T> *left, *right, *parent;

    // 构造函数
    Node(T key) : key(key), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

template <typename T>
class RBTree {
private:
    Node<T>* root;
    Node<T>* NIL; // 哨兵节点，代表所有叶子节点

    // 左旋
    void leftRotate(Node<T>* x) {
        Node<T>* y = x->right;
        x->right = y->left;
        if (y->left != NIL) {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {
            this->root = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }

    // 右旋
    void rightRotate(Node<T>* x) {
        Node<T>* y = x->left;
        x->left = y->right;
        if (y->right != NIL) {
            y->right->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {
            this->root = y;
        } else if (x == x->parent->right) {
            x->parent->right = y;
        } else {
            x->parent->left = y;
        }
        y->right = x;
        x->parent = y;
    }

    // 插入后的修复操作
    void insertFixup(Node<T>* k) {
        Node<T>* u; // Uncle node
        // 循环直到k的父节点是黑色或k是根节点
        while (k->parent->color == RED) {
            // 如果父节点是祖父节点的右孩子
            if (k->parent == k->parent->parent->right) {
                u = k->parent->parent->left; // 叔叔是左孩子
                // 情况一: 叔叔是红色
                if (u->color == RED) {
                    u->color = BLACK;
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    k = k->parent->parent; // 将k指向祖父，继续向上检查
                } else { // 情况二: 叔叔是黑色
                    // "之"字形 (RL) -> 转为 "一"字形 (RR)
                    if (k == k->parent->left) {
                        k = k->parent;
                        rightRotate(k);
                    }
                    // "一"字形 (RR)
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    leftRotate(k->parent->parent);
                    break; // 修复完成
                }
            } else { // 如果父节点是祖父节点的左孩子 (与上面对称)
                u = k->parent->parent->right; // 叔叔是右孩子
                // 情况一: 叔叔是红色
                if (u->color == RED) {
                    u->color = BLACK;
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    k = k->parent->parent;
                } else { // 情况二: 叔叔是黑色
                    // "之"字形 (LR) -> 转为 "一"字形 (LL)
                    if (k == k->parent->right) {
                        k = k->parent;
                        leftRotate(k);
                    }
                    // "一"字形 (LL)
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    rightRotate(k->parent->parent);
                    break;
                }
            }
            if (k == root) {
                break;
            }
        }
        root->color = BLACK; // 规则2: 根节点永远是黑色
    }

    // 递归中序遍历
    void inOrderHelper(Node<T>* node) {
        if (node != NIL) {
            inOrderHelper(node->left);
            std::cout << node->key << "(" << (node->color == RED ? "R" : "B") << ") ";
            inOrderHelper(node->right);
        }
    }
    
    // 递归销毁
    void destroyTree(Node<T>* node) {
        if (node != NIL) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

    private:
    // 用一棵子树 v 替换另一棵子树 u
    void transplant(Node<T>* u, Node<T>* v) {
        if (u->parent == nullptr) {
            root = v;
        } else if (u == u->parent->left) {
            u->parent->left = v;
        } else {
            u->parent->right = v;
        }
        v->parent = u->parent;
    }

    // 查找子树的最小节点
    Node<T>* findMin(Node<T>* node) {
        while (node->left != NIL) {
            node = node->left;
        }
        return node;
    }

    // 删除后的修复操作
    void deleteFixup(Node<T>* x) {
        Node<T>* w; // 兄弟节点
        while (x != root && x->color == BLACK) {
            if (x == x->parent->left) { // x 是左孩子
                w = x->parent->right;
                // 情况一: 兄弟是红色
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    leftRotate(x->parent);
                    w = x->parent->right;
                }
                // 情况二: 兄弟是黑色，且兄弟的两个孩子都是黑色
                if (w->left->color == BLACK && w->right->color == BLACK) {
                    w->color = RED;
                    x = x->parent; // 问题上移
                } else {
                    // 情况三: 兄弟是黑色，近侧孩子是红色
                    if (w->right->color == BLACK) {
                        w->left->color = BLACK;
                        w->color = RED;
                        rightRotate(w);
                        w = x->parent->right;
                    }
                    // 情况四: 兄弟是黑色，远侧孩子是红色
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->right->color = BLACK;
                    leftRotate(x->parent);
                    x = root; // 修复完成，跳出循环
                }
            } else { // x 是右孩子 (对称操作)
                w = x->parent->left;
                // 情况一
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    rightRotate(x->parent);
                    w = x->parent->left;
                }
                // 情况二
                if (w->right->color == BLACK && w->left->color == BLACK) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    // 情况三
                    if (w->left->color == BLACK) {
                        w->right->color = BLACK;
                        w->color = RED;
                        leftRotate(w);
                        w = x->parent->left;
                    }
                    // 情况四
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->left->color = BLACK;
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = BLACK; // 确保根节点或修复后的节点是黑色
    }
    
    // 递归查找并删除节点
    void deleteNodeHelper(Node<T>* z) {
        Node<T> *x, *y;
        y = z;
        Color y_original_color = y->color;

        if (z->left == NIL) {
            x = z->right;
            transplant(z, z->right);
        } else if (z->right == NIL) {
            x = z->left;
            transplant(z, z->left);
        } else {
            y = findMin(z->right); // 找到中序后继
            y_original_color = y->color;
            x = y->right;
            if (y->parent == z) {
                x->parent = y; // 很重要，即使x是NIL
            } else {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        delete z;
        if (y_original_color == BLACK) {
            deleteFixup(x);
        }
    }

    // 查找节点
    Node<T>* searchTreeHelper(T key) {
        Node<T>* node = root;
        while(node != NIL && key != node->key) {
            if (key < node->key) {
                node = node->left;
            } else {
                node = node->right;
            }
        }
        return node;
    }

public:
    RBTree() {
        NIL = new Node<T>(T()); // 哨兵节点
        NIL->color = BLACK;
        NIL->left = nullptr;
        NIL->right = nullptr;
        root = NIL;
    }
    
    ~RBTree() {
        destroyTree(this->root);
        delete NIL;
    }

    void insert(T key) {
        Node<T>* node = new Node<T>(key);
        node->parent = nullptr;
        node->left = NIL;
        node->right = NIL;
        node->color = RED; // 新节点总是红色

        Node<T>* y = nullptr;
        Node<T>* x = this->root;

        // 1. 标准BST插入
        while (x != NIL) {
            y = x;
            if (node->key < x->key) {
                x = x->left;
            } else {
                x = x->right;
            }
        }

        node->parent = y;
        if (y == nullptr) {
            root = node;
        } else if (node->key < y->key) {
            y->left = node;
        } else {
            y->right = node;
        }

        // 如果新插入的是根节点
        if (node->parent == nullptr) {
            node->color = BLACK;
            return;
        }

        // 如果祖父节点是空，也无需修复
        if (node->parent->parent == nullptr) {
            return;
        }

        // 2. 调用修复函数
        insertFixup(node);
    }

    void deleteNode(T key) {
        Node<T>* z = searchTreeHelper(key);
        if (z == NIL) {
            std::cout << "无法找到键值为 " << key << " 的节点" << std::endl;
            return;
        }
        deleteNodeHelper(z);
    }

    void printInOrder() {
        inOrderHelper(this->root);
        std::cout << std::endl;
    }
};
#endif // RBTREE_H