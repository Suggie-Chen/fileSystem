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
//    if (node == nullptr)//��ͷ
//    {
//        return 0;
//    }
//    else if (node->name == tobefind)//ƥ������һ��
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
            return nullptr;//�����ҵ�·��������
        }
        if (temp->name == res[i])//����һ��ƥ������
        {
            temp = temp->child;//������һ���ƥ��
        }
        else
        {
            temp = temp->sibling;//��ͬ����ֵܽڵ�
        }
    }
    return temp;
}

int FileSystem::create(const Tree &t, string path,  string name,  int type)//��path�´����ļ����ļ���name
{
   
    TreeNode * pnode=matchPath(path, t);
    TreeNode newnode;
 
	return 1;
}

vector<string> FileSystem::split(const string& str, const string& delim)//�ַ�����Ƭ
{
    vector<string> res;
    if ("" == str) return res;
    //�Ƚ�Ҫ�и���ַ�����string����ת��Ϊchar*����  
    char* strs = new char[str.length() + 1]; //��Ҫ����  
    strcpy_s(strs,strlen(strs)+1, str.c_str());

    char* d = new char[delim.length() + 1];
    strcpy_s(d, strlen(d)+1,delim.c_str());

    char* buf=NULL;
    char* p = strtok_s(strs, d,&buf);
    while (p) {
        string s = p; //�ָ�õ����ַ���ת��Ϊstring����  
        res.push_back(s); //����������  
        p = strtok_s(NULL, d,&buf);
    }
    return res;
}
