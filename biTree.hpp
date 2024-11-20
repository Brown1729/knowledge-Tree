/**
 * -*- coding: UTF-8 -*-（if u open this file in GBK, all chinese characters are chaos.)
 * @brief 一个简单的二叉树标准容器，混杂着一些专用于处理树的算法。
 * @author Brown1729
 * @date 2024/11/16
 * @version 1.0.0
 * @attention 部分代码可能不支持MSVC，请使用gcc 8.1版本进行编译。
 */

#include <iostream>
#ifndef _BI_TREE
#define _BI_TREE
template <class T>
struct node
{
    T data;
    double weight;
    // parent 用于判断是左连接还是右连接。
    node<T> *parent, *lchild, *rchild;
};

template <class T>
class ListBiTree
{
public:
    // 调试用的构造函数，实际的知识树中并未用到该方法。
    ListBiTree(const T pre[], const T &truncPoint, const T &endPoint, const double *weights = nullptr) // 定义中断点和停止点
        : _truncPoint(truncPoint), _endPoint(endPoint)
    {
        nodeNum = 0;
        createListBiTreePre(nullptr, biTree, pre, nodeNum, weights);
    }

    ListBiTree()
    {
        nodeNum = 0;
        biTree = nullptr;
    }

    ~ListBiTree()
    {
        clear(biTree);
    }

    // 过晚想到写一个判断empty函数，导致许多都是间接判断的。
    bool empty()
    {
        return !biTree;
    }

    node<T> *&getRoot()
    {
        return biTree;
    }

    node<T> *&insertRightRight(node<T> *&p, const T &data, const double weight = 0)
    //
    // 将新结点插入p结点的右侧，再将p结点原来的结点放在新结点的右侧。
    // 目的是为了能用孩子兄弟表示法表示树的时候，可以插入同级的结点，并且将后续的结点进行移动。
    // 返回值为插入的新结点的地址。
    // 【注意】考虑空树。
    {
        // 空树
        if (!p)
        {
            p = new node<T>{data, weight, nullptr, nullptr, nullptr};
            return p;
        }

        node<T> *temp = new node<T>{data, weight, p, nullptr, p->rchild};
        // struct的默认构造函数是memberwised copy。
        if (p->rchild)
        {
            p->rchild->parent = temp;
        }
        p->rchild = temp;
        return p->rchild;
    }

    node<T> *&insertLeftLeft(node<T> *&p, const T &data, const double weight = 0)
    //
    // 将新结点插入p的左边，并且将左节点，插在新结点的左边。
    //
    {
        // 空树
        if (!p)
        {
            p = new node<T>{data, weight, nullptr, nullptr, nullptr};
            return p;
        }

        node<T> *temp = new node<T>{data, weight, p, p->lchild, nullptr};
        if (p->rchild)
        {
            p->lchild->parent = temp;
        }
        p->lchild = temp;
        return p->lchild;
    }

    node<T> *&insertLeftRight(node<T> *&p, const T &data, const double weight = 0)
    {
        if (!p)
        {
            p = new node<T>{data, weight, nullptr, nullptr, nullptr};
            return p;
        }

        node<T> *temp = new node<T>{data, weight, p, nullptr, p->lchild};
        if (p->lchild)
        {
            p->lchild->parent = temp;
        }
        p->lchild = temp;
        return p->lchild;
    }

    node<T> *&insertRightLeft(node<T> *&p, const T &data, const double weight = 0)
    {
        if (!p)
        {
            p = new node<T>{data, weight, nullptr, nullptr, nullptr};
            return p;
        }

        node<T> *temp = new node<T>{data, weight, p, p->rchild, nullptr};
        // struct的默认构造函数是memberwised copy。
        if (p->rchild)
        {
            p->rchild->parent = temp;
        }
        p->rchild = temp;
        return p->rchild;
    }

///////////////////////////////////////////////////
// 各种遍历算法。
    void levelOrder()
    // 利用队列实现二叉树的中续遍历
    {
        node<T> *queue[nodeNum];
        int rear = 0, front = 0;
        node<T> *p = biTree;
        queue[rear++] = p;
        while (rear != front)
        {
            p = queue[front];
            front = (front + 1) % nodeNum;
            std::cout << p->data;
            if (p->lchild)
            {
                queue[rear] = p->lchild;
                rear = (rear + 1) % nodeNum;
            }
            if (p->rchild)
            {
                queue[rear] = p->rchild;
                rear = (rear + 1) % nodeNum;
            }
        }
    }

    void inOrderRecur()
    {
        inOrderRecur(biTree);
    }

    void preOrderRecur()
    {
        preOrderRecur(biTree);
    }

    void postOrderRecur()
    {
        postOrderRecur(biTree);
    }
//////////////////////////////////////////////////////////////////////////

    // 如果temp设在函数内，那么temp就无法返回，我发现这是我用类设计的通病！
    void preOrderFind(const T &data, node<T> *&t, node<T> *&temp)
    {
        if (t != nullptr)
        {
            if (t->data == data)
            {
                temp = t;
            }
            preOrderFind(data, t->lchild, temp);
            preOrderFind(data, t->rchild, temp);
        }
    }

    void clear(node<T> *&t)
    {
        if (t != nullptr)
        {
            clear(t->lchild);
            clear(t->rchild);
            delete t;
            t = nullptr;
        }
    }

protected:
    node<T> *biTree;
    // 是否需要计算树的总的节点数？？我觉得需要
    int nodeNum;
    T _endPoint;
    T _truncPoint;

    void createListBiTreePre(node<T> *parent, node<T> *&t, const T pre[], int &n, const double *weights = nullptr)
    // 如果pre中遇到'#'，那么这一格的weights就随便放，不会被记录进去。
    // parent是父结点
    // t是子节点
    {
        const T &ch = pre[n++];
        // 这里的n++不能放到末尾，否则就是无穷的a。
        if (ch == _endPoint)
            return;
        if (ch != _truncPoint)
        {
            // parent 不可设成常量指针，因为类内的parent是普通指针，常量指针无法赋值给普通指针。
            t = new node<T>{ch, weights ? weights[n - 1] : 0, parent, nullptr, nullptr};
            // t = new node<T>;
            // t->data = ch;
            // t->weight = weights ? weights[n] : 0;
            // t->parent = parent;
            // t->lchild = nullptr;
            // t->rchild = nullptr;
            createListBiTreePre(t, t->lchild, pre, n, weights);
            createListBiTreePre(t, t->rchild, pre, n, weights);
        }
        else
        {
            t = nullptr;
        }
    }

    void inOrderRecur(node<T> *&t)
    {
        if (t != nullptr)
        {
            inOrderRecur(t->lchild);
            std::cout << t->data;
            inOrderRecur(t->rchild);
        }
    }

    void preOrderRecur(node<T> *&t)
    {
        if (t != nullptr)
        {
            std::cout << t->data;
            preOrderRecur(t->lchild);
            preOrderRecur(t->rchild);
        }
    }

    void postOrderRecur(node<T> *&t)
    {
        if (t != nullptr)
        {
            postOrderRecur(t->lchild);
            postOrderRecur(t->rchild);
            std::cout << t->data;
        }
    }
};
#endif

// int main()
// {
//     char test[] = "ABC##D#E###"; // 抽象，为什么用字符串初始化的就是const类型。
//     ListBiTree<char> tree(test, '#', '\0');
//     tree.preOrderRecur();
//     std::cout << std::endl;
//     node<char> *p = tree.preOrderFind('A');
//     p->insertRightRight('F');
//     tree.levelOrder();
//     std::cout << std::endl;
//     system("pause");
// }