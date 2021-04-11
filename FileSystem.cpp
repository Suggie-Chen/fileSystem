#include "FileSystem.h"
#include<vector>
#include <iostream>
FileSystem::FileSystem()
{
	root = nullptr;
}

//void preOrderTraveral(TreeNode node) {
//    if (node == null) {
//        return;
//    }
//    System.out.print(node.data + " ");
//    preOrderTraveral(node.leftChild);
//    preOrderTraveral(node.rightChild);
//}

//int recursion(TreeNode * node,vector<string> tobefind)
//{
//    if (node == nullptr)//到头
//    {
//        return 0;
//    }
//    else if (node->name == tobefind)//匹配上了一段
//    {
//        recursion(node)
//    }
//    else
//    {
//        recursion(node)
//    }
//}
TreeNode* FileSystem::matchPath(string path, const Tree &t)
{
    vector<string> res = split(path, "\\");
    for (int i = 0; i < res.size(); i++)
    {
        std::cout << res[i] << " ";
    }

    TreeNode* temp = t;
    for (int i = 0; i < res.size(); i++)
    {
        if (temp == nullptr)
        {
            return nullptr;//待查找的路径不存在
        }
        if (temp->name == res[i])//在这一层匹配上了
        {
            temp = temp->child;//进入下一层的匹配
        }
        else
        {
            temp = temp->sibling;//找同层的兄弟节点
        }
    }
    return temp;
}

int FileSystem::create(const Tree &t, string path,  string name,  int type)//在path下创建文件或文件夹name
{
   
    TreeNode * pnode=matchPath(path, t);
    TreeNode newnode;
 
	return 1;
}

vector<string> FileSystem::split(const string& str, const string& delim)//字符串切片
{
    vector<string> res;
    if ("" == str) return res;
    //先将要切割的字符串从string类型转换为char*类型  
    char* strs = new char[str.length() + 1]; //不要忘了  
    strcpy_s(strs,strlen(strs)+1, str.c_str());

    char* d = new char[delim.length() + 1];
    strcpy_s(d, strlen(d)+1,delim.c_str());

    char* buf=NULL;
    char* p = strtok_s(strs, d,&buf);
    while (p) {
        string s = p; //分割得到的字符串转换为string类型  
        res.push_back(s); //存入结果数组  
        p = strtok_s(NULL, d,&buf);
    }
    return res;
}
