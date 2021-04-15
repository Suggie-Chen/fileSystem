#include<vector>
#include<stack>
#include <iostream>
#include "FileSystem.h"
#include "FCB.h"
FileSystem::FileSystem()
{
    dirTree = new TreeNode(1, "root", nullptr, nullptr, nullptr);//创建一个叫root的文件夹
}

void FileSystem::postOrderDelSubTree(Tree &t)//后序遍历删除文件子树
{
    if (t == nullptr)
    {
        return;
    }
    if (t->child != nullptr)
    {
        postOrderDelSubTree(t->child);
    }
    if (t->sibling != nullptr)
    {
        postOrderDelSubTree(t->sibling);
    }

    delete t;
    t = nullptr;
   /* if (t != nullptr)
    {
        
        postOrderDelSubTree(t->child);
        postOrderDelSubTree(t->sibling);
        delete t;
    }*/
}

void FileSystem::preOrderTraverse(const Tree& t,int depth,vector<string> &s)
{
    if (t != nullptr)
    {
        string temp;
        for (int i = 0; i < depth; i++)
        {
            temp = temp + "    ";
        }
        s.push_back(temp + t->name);
        preOrderTraverse(t->child,depth+1,s);
        preOrderTraverse(t->sibling,depth,s);
    }
}

int FileSystem::printDir()//打印文件目录，即深搜打印树
{
    cout << "------------------------------------------------" << endl;
    cout << "文件目录如下：" << endl;
   vector<string> s;
    preOrderTraverse(dirTree,0,s);
    for (int i = 0; i < s.size(); i++)
    {
        cout << s[i] << endl;
    }
    cout << "------------------------------------------------" << endl;
    return 0;
    
}
int FileSystem::del(string path,string subtreeroot)
{
    TreeNode* pnode = matchPath(path, dirTree);
    if (pnode == nullptr)
    {
        cout << "删除节点失败，该路径不存在" << endl;
    }
    else
    {
        TreeNode* lastptr = pnode;
        TreeNode* currentptr = pnode->child;
      
        while (currentptr != nullptr)
        {
            if (currentptr->name == subtreeroot)
            {
                postOrderDelSubTree(currentptr);
                cout << "删除子树成功！" << endl;
                return 0;
            }
            lastptr = currentptr;
            currentptr = currentptr->sibling;
            
        }
        if (currentptr == nullptr)
        {
            cout << "删除子树失败，该目录下不存在该子树" << endl;
            return 1;
        }
    }
}



TreeNode* FileSystem::matchPath(string path, const Tree &t)//在树t上找一条path，返回末尾的节点
{
    vector<string> res = split(path, "\\");
    TreeNode* temp = t;
    TreeNode* lastptr = nullptr;
    for (int i = 0; i < res.size(); i++)
    {
        if (temp->name == res[i]&&temp->type==1)//在这一层匹配上了
        {
            lastptr = temp;
            if (i < res.size() - 1)
            {
                temp = temp->child;//进入下一层的匹配
            }
        }
        else//在当前层从左到右找
        {
            while (temp != nullptr)
            {
                if (temp->name == res[i]&&temp->type==1)//在当前层找到了
                {
                    break;
                }
                else
                {
                    temp = temp->sibling;
                }
             }
            if (temp == nullptr)
            {
                cout << "待查找的路径不存在" << endl;  
                return temp;
            }
        }
    }
    cout << "路径匹配成功！" << endl;
    return temp;
}

int FileSystem::create(string path,  string name,  int type)//在path下创建文件或文件夹name
{

    TreeNode * pnode=matchPath(path, dirTree);
    if (pnode == nullptr)
    {
        cout << "创建新节点失败！" << endl;
        return 2;
    }
    if (pnode->child == nullptr)//该目录下还没有东西时
    {
        pnode->child = new TreeNode(type, name, pnode, nullptr, nullptr);
    }
    else
    {
        TreeNode* currentptr = pnode->child;
        TreeNode* lastptr = nullptr;
        while (currentptr!=nullptr)
        {
            if (currentptr->name == name)
            {
                cout << "当前目录下存在同名文件/文件夹，创建新节点失败！" << endl;
                return 2;
            }
            lastptr = currentptr;
            currentptr = currentptr->sibling;
        }
        lastptr->sibling= new TreeNode(type, name, pnode, nullptr, nullptr);
    }
    cout << "创建好啦！" << endl;
	return 1;
}

vector<string> FileSystem::split(const string& str, const string& delim)//字符串切片
{
    vector<string> res;
    if ("" == str) return res;
    //先将要切割的字符串从string类型转换为char*类型  
    int len1 = str.length() + 1;//要加上一个\0
    char* strs = new char[len1]; //不要忘了  
    strcpy_s(strs,len1, str.c_str());

    int len2 = delim.length() + 1;
    char* d = new char[len2];
    strcpy_s(d, len2,delim.c_str());

    char* buf=NULL;
    char* p = strtok_s(strs, d,&buf);
    while (p) {
        string s = p; //分割得到的字符串转换为string类型  
        res.push_back(s); //存入结果数组  
        p = strtok_s(NULL, d,&buf);
    }
    delete [] strs;
    delete [] d;

    for (int i = 0; i < res.size(); i++)
    {
        std::cout << res[i] << " ";
    }
    cout << "得到的字段数：" << res.size() << endl;
    return res;
}
