/**
 * -*- coding: utf-8 -*-（if u open this file in GBK, all chinese characters are chaos.)
 * @brief 构建的知识树
 * @author Brown1729
 * @date 2024/11/16
 * @version 1.0.0
 * @attention 部分代码可能不适用MSVC，请使用gcc 8.1版本进行编译
 * @bug 1.由于我的电脑采用的GBK编码，所以生成的控制台默认采用GBK，而此处的中文是utf-8，
 *      所以中文都是乱码，那么我这棵知识树只好以英文的方式建立，甚至连制表符都不能用。
 */

#include "Tree.hpp"
#include <string>
#include <stack>
#include <fstream>
#include <vector>
using namespace std;
using strNode = node<string> *;

class KnowledgeTree : public Tree<string>
{
public:
    void printBelowLayer(strNode p)
    {
        int i = 0;
        for (strNode temp = p->lchild; temp; temp = temp->rchild, ++i)
        {
            cout << i + 1 << ": [" << temp->data << "] : " << temp->weight << endl;
        }
    }

    void printFullTree(strNode p, const double weight = 0)
    {
        int n = 0; // 用来表示递归深度
        if (p != getRoot())
        {
            n = 1;
            cout << "[" << p->data << "]" << endl;
        }
        printFullTree(p->lchild, n, weight);
    }

    void printFullTree(strNode p, int &n, const double weight = 0)
    {
        if (p && p->weight >= weight)
        {
            for (int i = 0; i < n; ++i)
            {
                cout << "---";
            }
            cout << "[" << p->data << "]" << endl;
            ++n;
            printFullTree(p->lchild, n, weight);
            --n;
            printFullTree(p->rchild, n, weight);
        }
    }

    // 找得到返回结点，找不到返回nullptr
    strNode getNodeFromIndex(const strNode above, const int index)
    {
        int i = 1;
        strNode temp;
        for (temp = above->lchild; temp && i < index; temp = temp->rchild, ++i)
            ;
        return temp;
    }

    // 找得到返回数字，找不到返回0
    int getIndexFromNode(const strNode above, const strNode targe)
    {
        int i = 1;
        strNode temp;
        for (temp = above->lchild; temp && temp != targe; temp = temp->rchild, ++i)
            ;
        if (!temp)
        {
            return 0;
        }
        return i;
    }

    // 搜索data的同时利用栈记录结构
    // 如果找不到data，target还是原来的值，所以需要target传一个nullptr进去
    void findWithStack(stack<strNode> &nodeStack, const string &data, strNode &target)
    {
        stack<strNode> tempNodeStack;
        tempNodeStack.push(getRoot());
        _findWithStack(nodeStack, tempNodeStack, getRoot()->lchild, data, target);
    }

    void _findWithStack(stack<strNode> &nodeStack, stack<strNode> &tempNodeStack, strNode &t, const string &data, strNode &target)
    {
        if (t)
        {
            tempNodeStack.push(t);
            _findWithStack(nodeStack, tempNodeStack, t->lchild, data, target);
            tempNodeStack.pop();
            _findWithStack(nodeStack, tempNodeStack, t->rchild, data, target);
            if (t->data == data)
            {
                target = t;
                // 这一步显然不是最优
                nodeStack = tempNodeStack;
            }
        }
    }

    void createListBiTreePreWithIfile(ifstream &ifile, strNode parent, strNode &t, const string &truncPoint)
    {
        string data;
        string str;
        double weight;
        getline(ifile, data);
        getline(ifile, str);
        weight = stod(str);
        if (ifile.eof())
        {
            return;
        }
        if (data != truncPoint)
        {
            // parent 不可设成常量指针，因为类内的parent是普通指针，常量指针无法赋值给普通指针。
            t = new node<string>{data, weight, parent, nullptr, nullptr};
            createListBiTreePreWithIfile(ifile, t, t->lchild, truncPoint);
            createListBiTreePreWithIfile(ifile, t, t->rchild, truncPoint);
        }
        else
        {
            t = nullptr;
        }
    }

    void preOrderOutput(strNode t, ofstream &ofile, const string &truncPoint)
    {
        if (t)
        {
            ofile << t->data << "\n";
            ofile << t->weight << "\n";
            preOrderOutput(t->lchild, ofile, truncPoint);
            preOrderOutput(t->rchild, ofile, truncPoint);
        }
        else
        {
            ofile << truncPoint << "\n"
                  << 0 << "\n";
        }
    }

private:
    friend class ListBiTree<string>;
    friend class Tree<string>;
};

int main()
{
    // nodeStack中存储的是目前的结点，主要操作是在这个结点之下进行的。
    stack<strNode> nodeStack;
    KnowledgeTree kt;
    nodeStack.push(kt.getRoot());
    cout << "-----------Knowledge Tree-------------" << endl;
    int choice = 0;

    // 1.用于插入
    // 2.用于重命名
    // 3.用于删除
    // 4.用于输出树的全貌
    int place = 0;

    // 1.用于向树中添加新的结点
    // 2.用于查找信息
    string newPoint;

    // 1.用于向树中添加新的结点
    // 2.用于筛选重点
    double newPointWeight = 0;

    // 用于记录查找到的结点
    strNode temp;
    do
    {
        if (nodeStack.top() == kt.getRoot())
        {
            cout << "The root of the tree:" << endl;
        }
        else
        {
            cout << "Following are point(s) below point [" << nodeStack.top()->data << "]" << endl;
        }
        kt.printBelowLayer(nodeStack.top());
        cout << endl;
        cout << "Please choose the number in front of operation." << endl;
        cout << "1: Insert new point at the top." << endl;
        cout << "2: Insert new point at the end." << endl;
        cout << "3: Insert new point after." << endl;
        cout << "4: Rename a point." << endl;
        cout << "5: Change weight of point." << endl;
        cout << "6: Delete a point." << endl;
        cout << "7: Get into point." << endl;
        cout << "8: Back." << endl;
        cout << "9: Find." << endl;
        cout << "10: Show the system below." << endl;
        cout << "11: Show the highlights below." << endl;
        cout << "12: Import point at the end." << endl;
        cout << "13: Save the point." << endl;
        cout << "14: Quit." << endl;
        // 其他函数结束之后cin会默认忽略前面的换行或空格
        cin >> choice;
        cin.ignore(); // 忽略掉换行符
        switch (choice)
        {
        case 1: // insert new point at the top.
        {
            cout << "Please enter the new point." << endl;
            getline(cin, newPoint);
            cout << "Please enter its weight." << endl;
            cin >> newPointWeight;
            kt.insertBelowFront(nodeStack.top(), newPoint, newPointWeight);
            break;
        }
        
        case 2: // insert new point at the end;
        {
            cout << "Please enter the new point." << endl;
            getline(cin, newPoint);
            cout << "Please enter its weight." << endl;
            cin >> newPointWeight;
            kt.insertBelowBack(nodeStack.top(), newPoint, newPointWeight);
            break;
        }

        case 3:
        {
            if (!(nodeStack.top()->lchild))
            {
                cout << "[Warning]: No point here." << endl;
                break;
            }
            cout << "Please enter the new point." << endl;
            getline(cin, newPoint);
            cout << "Please enter its weight." << endl;
            cin >> newPointWeight;
            cout << "Please choose the place you want to insert." << endl;
            cin >> place;
            temp = kt.getNodeFromIndex(nodeStack.top(), place);
            if (!temp)
            {
                cout << "[Warning]: No point number [" << place << "] here." << endl;
                system("pause");
                break;
            }
            kt.insertAfter(temp, newPoint, newPointWeight);
            cout << "[Result]: Insert successfully." << endl;
            break;
        }

        case 4: // rename a point
        {
            if (!(nodeStack.top()->lchild))
            {
                cout << "[Warning]: No point here." << endl;
                system("pause");
                break;
            }
            cout << "Please choose the point you want to rename." << endl;
            cin >> place;
            cin.ignore(); // 忽略掉换行符
            temp = kt.getNodeFromIndex(nodeStack.top(), place);
            if (!temp)
            {
                cout << "[Warning]: No point number [" << place << "] here." << endl;
                system("pause");
                break;
            }
            cout << "Please enter the new name." << endl;
            getline(cin, newPoint);
            temp->data = newPoint;
            cout << "[Result]: Renaming Successfully." << endl;
            break;
        }

        case 5: // change weight of point
        {
            // 高度重复
            if (!(nodeStack.top()->lchild))
            {
                cout << "[Warning]: No point here." << endl;
                system("pause");
                break;
            }
            cout << "Please choose the point whose weight you want to change." << endl;
            cin >> place;
            cin.ignore(); // 忽略掉换行符
            temp = kt.getNodeFromIndex(nodeStack.top(), place);
            if (!temp)
            {
                cout << "[Warning]: No point number [" << place << "] here." << endl;
                system("pause");
                break;
            }
            // 高度重复

            cout << "Please enter new weight." << endl;
            cin >> newPointWeight;
            cin.ignore(); // 忽略掉换行符
            temp->weight = newPointWeight;
            break;
        }

        case 6: // delete a point
        {
            if (!(nodeStack.top()->lchild))
            {
                cout << "[Warning]: No point here." << endl;
                system("pause");
                break;
            }
            cout << "Please choose the point you want to delete.('0' to quit)" << endl;
            cin >> place;
            cin.ignore(); // 忽略换行符
            if (!place)
            {
                break;
            }
            temp = kt.getNodeFromIndex(nodeStack.top(), place);
            if (!temp)
            {
                cout << "[Warning]: No point number [" << place << "] here." << endl;
                system("pause");
                break;
            }

            kt.deleteNode(temp);
            break;
        }

        case 7: // get into point
        {
            if (!(nodeStack.top()->lchild))
            {
                cout << "[Warning]: No point(s) here." << endl;
                system("pause");
                break;
            }
            cout << "Please choose the point you want to get into." << endl;
            cin >> place;
            cin.ignore(); // 忽略掉换行符
            temp = kt.getNodeFromIndex(nodeStack.top(), place);
            if (!temp)
            {
                cout << "[Warning]: No point number [" << place << "] here." << endl;
                system("pause");
                break;
            }
            nodeStack.push(temp);
            break;
        }

        case 8: // back
        {
            if (nodeStack.top() == kt.getRoot())
            {
                cout << "[Warning]: It's all the way to the bottom." << endl;
                system("pause");
                break;
            }
            nodeStack.pop();
            break;
        }

        case 9: // find
        {
            // 如果用递归方式查找，只能得到一个结点，无法得到有关该结点的体系。
            // Find会跳出目前所有的工作。
            cout << "Please enter the point you want to find." << endl;
            getline(cin, newPoint);
            temp = nullptr;
            kt.findWithStack(nodeStack, newPoint, temp);
            if (!temp)
            {
                cout << "[Finding Result]: Can't find point: " << newPoint << endl;
                system("pause");
                break;
            }
            cout << "[Finding Result]: Index of [" << newPoint << "] is [" << kt.getIndexFromNode(nodeStack.top(), temp) << "] in following list." << endl;
            break;
        }

        case 10: // show the system below
        {
            cout << "Please choose the point you want to view over('0' is whole tree)." << endl;
            cin >> place;
            cout << endl;
            cout << "Below is the structure" << endl;
            if (!place)
            {
                kt.printFullTree(kt.getRoot());
                system("pause");
                break;
            }
            temp = kt.getNodeFromIndex(nodeStack.top(), place);
            kt.printFullTree(temp);
            system("pause");
            break;
        }

        case 11: // show the highlights below
        {
            cout << "Please choose the point you want to view over('0' is whole tree)." << endl;
            cin >> place;
            cout << "Please enter filter weight" << endl;
            cin >> newPointWeight;

            cout << endl;
            cout << "Below is the structure under weight [" << newPointWeight << "]" << endl;
            if (!place)
            {
                kt.printFullTree(kt.getRoot(), newPointWeight);
                system("pause");
                break;
            }
            temp = kt.getNodeFromIndex(nodeStack.top(), place);
            if (!temp)
            {
                cout << "[Warning]: No point number [" << place << "] here." << endl;
                system("pause");
                break;
            }
            kt.printFullTree(temp, newPointWeight);
            system("pause");
            break;
        }

        case 12: // import
        {
            cout << "Please enter the file where contain the points." << endl;
            string fileName;
            getline(cin, fileName);
            // 二叉树的扩充结点为'#'
            // 利用\n分割数据和权重，这是目前我能想到的最好的方法
            ifstream ifile(fileName.c_str(), ios::in);
            if (!ifile)
            {
                cout << "[Warning]: File: \"" << fileName << "\" doesn't exit." << endl;
                system("pause");
                break;
            }
            strNode newBiTree;
            kt.createListBiTreePreWithIfile(ifile, nullptr, newBiTree, "#");
            kt.insertBelowBack(nodeStack.top(), newBiTree);
            cout << "[Result]: Import successfully!" << endl;
            system("pause");
            break;
        }

        case 13: // save
        {
            if (kt.empty())
            {
                cout << "[Warning]: No points can be saved." << endl;
                system("pause");
                break;
            }
            cout << "Please enter the point(s) you want to save('0' is the whole tree)." << endl;
            cin >> place;
            cin.ignore(); // 删换行符
            cout << "Please enter the file name you want to save the tree." << endl;
            getline(cin, newPoint);
            ofstream ofile(newPoint.c_str(), ios::out);
            strNode leftTrunc; // 用来暂时记录要存的结点的右子树。先随便引用一个值
            if (!ofile)
            {
                cout << "[Warning]: File: \"" << newPoint << "\" is illegal." << endl;
                system("pause");
                break;
            }
            if (!place)
            {
                temp = kt.getRoot()->lchild;
                kt.preOrderOutput(temp, ofile, "#");
            }
            else
            {
                temp = kt.getNodeFromIndex(nodeStack.top(), place);
                leftTrunc = temp->rchild;
                temp->rchild = nullptr;
                kt.preOrderOutput(temp, ofile, "#");
                temp->rchild = leftTrunc;
            }
            cout << "[Result]: Save successfully!" << endl;
            system("pause");
            break;
        }

        default:
        {
            if (choice != 14)
            {
                cout << "[Warning]: The operation number [" << choice << "] is not available." << endl;
                system("pause");
                break;
            }
        }
        }

    } while (choice != 14);
}