#include<vector>
#include<stack>
#include <iostream>
#include "FileSystem.h"
#include "FCB.h"
FileSystem::FileSystem()
{
    dirTree = new TreeNode(1, "root", nullptr, nullptr, nullptr);//����һ����root���ļ���
}

void FileSystem::postOrderDelSubTree(Tree &t)//�������ɾ���ļ�����
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

int FileSystem::printDir()//��ӡ�ļ�Ŀ¼�������Ѵ�ӡ��
{
    cout << "------------------------------------------------" << endl;
    cout << "�ļ�Ŀ¼���£�" << endl;
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
        cout << "ɾ���ڵ�ʧ�ܣ���·��������" << endl;
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
                cout << "ɾ�������ɹ���" << endl;
                return 0;
            }
            lastptr = currentptr;
            currentptr = currentptr->sibling;
            
        }
        if (currentptr == nullptr)
        {
            cout << "ɾ������ʧ�ܣ���Ŀ¼�²����ڸ�����" << endl;
            return 1;
        }
    }
}



TreeNode* FileSystem::matchPath(string path, const Tree &t)//����t����һ��path������ĩβ�Ľڵ�
{
    vector<string> res = split(path, "\\");
    TreeNode* temp = t;
    TreeNode* lastptr = nullptr;
    for (int i = 0; i < res.size(); i++)
    {
        if (temp->name == res[i]&&temp->type==1)//����һ��ƥ������
        {
            lastptr = temp;
            if (i < res.size() - 1)
            {
                temp = temp->child;//������һ���ƥ��
            }
        }
        else//�ڵ�ǰ���������
        {
            while (temp != nullptr)
            {
                if (temp->name == res[i]&&temp->type==1)//�ڵ�ǰ���ҵ���
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
                cout << "�����ҵ�·��������" << endl;  
                return temp;
            }
        }
    }
    cout << "·��ƥ��ɹ���" << endl;
    return temp;
}

int FileSystem::create(string path,  string name,  int type)//��path�´����ļ����ļ���name
{

    TreeNode * pnode=matchPath(path, dirTree);
    if (pnode == nullptr)
    {
        cout << "�����½ڵ�ʧ�ܣ�" << endl;
        return 2;
    }
    if (pnode->child == nullptr)//��Ŀ¼�»�û�ж���ʱ
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
                cout << "��ǰĿ¼�´���ͬ���ļ�/�ļ��У������½ڵ�ʧ�ܣ�" << endl;
                return 2;
            }
            lastptr = currentptr;
            currentptr = currentptr->sibling;
        }
        lastptr->sibling= new TreeNode(type, name, pnode, nullptr, nullptr);
    }
    cout << "����������" << endl;
	return 1;
}

vector<string> FileSystem::split(const string& str, const string& delim)//�ַ�����Ƭ
{
    vector<string> res;
    if ("" == str) return res;
    //�Ƚ�Ҫ�и���ַ�����string����ת��Ϊchar*����  
    int len1 = str.length() + 1;//Ҫ����һ��\0
    char* strs = new char[len1]; //��Ҫ����  
    strcpy_s(strs,len1, str.c_str());

    int len2 = delim.length() + 1;
    char* d = new char[len2];
    strcpy_s(d, len2,delim.c_str());

    char* buf=NULL;
    char* p = strtok_s(strs, d,&buf);
    while (p) {
        string s = p; //�ָ�õ����ַ���ת��Ϊstring����  
        res.push_back(s); //����������  
        p = strtok_s(NULL, d,&buf);
    }
    delete [] strs;
    delete [] d;

    for (int i = 0; i < res.size(); i++)
    {
        std::cout << res[i] << " ";
    }
    cout << "�õ����ֶ�����" << res.size() << endl;
    return res;
}
