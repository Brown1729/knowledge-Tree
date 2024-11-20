/**
 * -*- coding: UTF-8 -*-（if u open this file in GBK, all chinese characters are chaos.)
 * @brief 利用孩子-兄弟表示法建立的树（森林）标准容器，带有许多专用的遍历、查找和插入算法
 * @author Brown1729
 * @date 2024/11/16
 * @version 1.0.0
 * @attention 部分代码可能不适用MSVC，请使用gcc 8.1版本进行编译
 */

#include "biTree.hpp"

// 如果可以手动给结点排序，操作的复杂程度不亚于上次那个链表。
// 如果需要将一棵树塞到另一颗树里（即树的移动），将会更加繁琐。
// 所以，目前只做一个能添加删除修改的版本。

// 空树，树的底层是两个无法避免的问题
// 空树判断用empty，树的底层判断用nullptr，如果有一个空置的根节点，这两种情况即可统一。

#ifndef _TREE
#define _TREE
template <class T>
class Tree
{
public:
    Tree()
    {
        // 起支撑作用的结点
        getRoot() = new node<T>{T(), 0, nullptr, nullptr, nullptr};
    }

    ~Tree()
    {
        tree.~ListBiTree();
    }

    void clear()
    {
        deleteNode(getRoot()->lchild);
    }

    // parent起作用了。
    void deleteNode(node<T> *&t)
    {
        // 删掉该结点的所有左结点。
        _clear(t->lchild);
        // 上一个结点右连接该结点。
        if (t->parent->rchild == t)
        {
            t->parent->rchild = t->rchild;
            if (t->rchild)
            {
                t->rchild->parent = t->parent;
            }
            delete t;
            return;
        }
        // 上一个结点左连接该结点。
        if (t->parent->lchild == t)
        {
            t->parent->lchild = t->rchild;
            if (t->rchild)
            {
                t->rchild->parent = t->parent;
            }
            delete t;
        }
    }

    bool empty()
    {
        return !(getRoot()->lchild) && !(getRoot()->rchild);
    }

    // 指向的是起支撑作用的根结点。
    node<T> *&getRoot()
    {
        return tree.getRoot();
    }

    void preOrderFind(const T &data, node<T> *&temp)
    {
        return tree.preOrderFind(data, getRoot()->lchild, temp);
    }

    node<T> *&insertBelowBack(node<T> *&p, const T &data, const double weight = 0)
    // 将新结点插入到p节点之下
    // 对应到二叉树中，就是插到p结点的左结点的最右边一个结点的右边。
    // 返回插入的结点。
    {
        if (!p)
        {
            std::cout << "Error in line:" << __LINE__ << std::endl;
            exit(1);
        }
        if (!(p->lchild))
        // 如果p结点没有左结点，那么直接插到p结点的左边。
        {
            return tree.insertLeftLeft(p, data, weight);
        }

        node<T> *temp;
        for (temp = p->lchild; temp->rchild; temp = temp->rchild)
            ;
        // 遍历p左结点的兄弟，一直到最后一个，即有节点为空。
        return tree.insertRightRight(temp, data, weight);
    }

    node<T> *&insertBelowBack(node<T> *&p, node<T> *&t)
    // 将新结点插入到p节点之下
    // 对应到二叉树中，就是插到p结点的左结点的最右边一个结点的右边。
    // 返回插入的结点。
    {
        if (!p)
        {
            std::cout << "Error in line:" << __LINE__ << std::endl;
            exit(1);
        }
        if (!(p->lchild))
        // 如果p结点没有左结点，那么直接插到p结点的左边。
        {
            p->lchild = t;
            t->parent = p;
            return t;
        }
        
        node<T> *temp;
        for (temp = p->lchild; temp->rchild; temp = temp->rchild)
            ;
        // 遍历p左结点的兄弟，一直到最后一个，即有节点为空。
        temp->lchild = t;
        t->parent = temp;
        return t;
    }

    // 将新结点插入p结点之下。
    // 但是插在p结点下的最前面。
    node<T> *&insertBelowFront(node<T> *&p, const T &data, const double weight = 0)
    {
        if (!p)
        {
            std::cout << "Error in line:" << __LINE__ << std::endl;
            exit(1);
        }
        return tree.insertLeftRight(p, data, weight);
    }

    // 加&，是为了能继续在这个后面添加。
    node<T> *&insertAfter(node<T> *&p, const T &data, const double weight = 0)
    {
        if (!p)
        {
            std::cout << "Error in line:" << __LINE__ << std::endl;
            exit(1);
        }
        if (p == getRoot())
        {
            std::cout << "Can't change root" << __LINE__ << std::endl;
            exit(1);
        }
        return tree.insertRightRight(p, data, weight);
    }

    // 下面是调试用打印算法。
    void preOrderRecur()
    {
        tree.preOrderRecur();
    }

protected:
    friend class ListBiTree<T>;
    ListBiTree<T> tree;

    void _clear(node<T> *&t)
    {
        if (t)
        {
            _clear(t->lchild);
            _clear(t->rchild);
            delete t;
            t = nullptr;
        }
    }
};
#endif

// int main()
// {
//     Tree<char> t;
//     t.insertBelowBack(t.insertAfter(t.insertBelowBack(t.getRoot(), 'A'), 'B'), 'C');
//     t.preOrderRecur();
//     std::cout << std::endl;
//     t.insertBelowFront(t.getRoot(), 'O');
//     node<char>* temp = nullptr;
//     t.preOrderFind('A', temp);
//     t.insertAfter(temp, 'a');
//     t.preOrderRecur();
//     system("pause");
// }