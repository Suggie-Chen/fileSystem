#include <iostream>
#include<vector>
#include<stack>
#include "FileSystem.h"
#include "FCB.h"

FileSystem::FileSystem()
{
    //���пռ��б�һ��ʼȫ������
    for (int i = 0; i < STORAGE_SIZE; i++)
    {
        freeSpaceList.push_back(i);
    }

    dirTree = new TreeNode(1, "root",0, nullptr, nullptr, nullptr);//����һ����root���ļ���
    swapAreaPath = "root\\temp";
    string temp=swapAreaPath.substr(5);
    create("root", temp, 1, 0);  //temp�����ڴ������ڵ��Ƚ��̵��ļ���

    //����һЩ��ʼ�ļ����ļ���
    create("root", "chenshuyi", 1,0);
    create("root", "OS����", 1,0);
    create("root\\OS����", "version1.0",2,4);
    create("root\\chenshuyi", "hw", 1,0);
    create("root\\chenshuyi\\hw", "hw1", 1,0);
    create("root\\chenshuyi\\hw", "hw2", 2,5);
    create("root\\chenshuyi\\hw", "hw2", 2,5);  //�����ã�֤��������ͬһ�ļ����´���ͬ���ļ����ļ���
    create("root\\chenshuyi\\hw", "hw3", 1,6);
    create("root\\chenshuyi\\hw\\hw1", "lalala", 2,10);
    create("root\\chenshuyi\\hw\\hw3", "lalala", 2, 1000);//�����ã�֤����治��ʱ�����ļ�ʧ��
    create("root\\chenshuyi\\hw\\hw3", "lalala", 2, 30);
    create("root\\chenshuyi\\hw\\hw2", "woc", 2,3); //�����ã�֤���������ļ��´����ļ�
    create("root\\zhangwangting", "zwt", 2,9);      //�����ã�֤�������ڲ����ڵ�·���´����ļ�
}

void FileSystem::test()
{
    printDir();
    printFreeSpaceList();

    search("lalala");
    rename("root\\chenshuyi\\hw\\hw3", "lalala", "yeyeye");
    rename("root\\chenshuyi\\hw\\hw3", "yeyeye", "yeyeye");
    /*rename("root\\chenshuyi\\hw\\hw1", "lalala", "yeyeye");*/

    printDir();

    del("root\\chenshuyi\\hw");
    printDir();
    printFreeSpaceList();
    search("lalala");

    swapToExternalStorage(123, 70);
    printDir();
    printFreeSpaceList();
    swapToMemory(123, 70);
    printDir();
    printFreeSpaceList();
}

void FileSystem::postOrderDelSubTree(Tree &t)//�������ɾ���ļ�����
{
    if (t == nullptr)
        return;

    postOrderDelSubTree(t->child);
    postOrderDelSubTree(t->sibling);
    //��ɾ�����ݿ�
    releaseExternalStorage(*t);
    //��ɾ��Ŀ¼��
    delete t;
    t = nullptr;
}

void FileSystem::preOrderTraverse(const Tree& t,int depth,vector<string> &s)//��������ļ�Ŀ¼��
{
    if (t != nullptr)
    {
        string temp;
        for (int i = 0; i < depth; i++)
        {
            temp = temp + "    ";
        }
        string seq=" ";
        for (int i = 0; i < t->size; i++)
        {
            seq = seq +" "+ to_string(t->location[i]);
        }
        s.push_back(temp + t->name + seq);
        preOrderTraverse(t->child,depth+1,s);
        preOrderTraverse(t->sibling,depth,s);
    }
}

int FileSystem::swapToExternalStorage(int pid, int size)
{
    int ans=create("root\\temp", "pid" + to_string(pid), 2,size);//���ļ���root\temp�´���һ����pidxxx���ļ�
    if (ans == 0)
    {
        cout << "�ɹ�������" << pid << "��" << size << "�黻������ϣ�" << endl;
    }
    else
    {
        cout<<"û�ܽ�����" << pid << "��" << size << "�黻������ϣ�" << endl;
    }
    return ans;
}

//ͼͼ�����������ڵ��������֮ǰ�ñ�֤��size���С�Ŀ����ڴ�
int FileSystem::swapToMemory(int pid, int size)
{
   //ɾ����ӦĿ¼�ڵ㣬�ͷ���Ӧ���
    string path = swapAreaPath+"\\pid"+to_string(pid);
    int ans=del(path);
    return ans;
}

int FileSystem::printDir()//��ӡ�ļ�Ŀ¼�������Ѵ�ӡ��
{
    cout << "-------------------------------------------------------------------------------------------" << endl;
    cout << "�ļ�Ŀ¼���£�" << endl;
   vector<string> s;
    preOrderTraverse(dirTree,0,s);
    for (int i = 0; i < s.size(); i++)
    {
        cout << s[i] << endl;
    }
    cout << "-------------------------------------------------------------------------------------------" << endl;
    return 0;
    
}

int FileSystem::del(string path)
{
    TreeNode*& pnode = matchPath(path);

    if (pnode == nullptr)
    {
        cout << "ɾ���ڵ�ʧ�ܣ���·��������" << endl;
        return 1;
    }
    else
    {
        postOrderDelSubTree(pnode);
        cout << "ɾ�������ɹ���" << endl;
        return 0;
    }
}

void FileSystem::preOrderSearch(TreeNode *t,string name,vector<TreeNode*> &resultList)
{
    if (t == nullptr)
    {
        return;
    }
    if (t->name == name)
    {
        resultList.push_back(t);
    }
    
    preOrderSearch(t->child, name,resultList);
    preOrderSearch(t->sibling, name, resultList);
}

int FileSystem::rename(string path, string oldname, string newname)
{
    TreeNode* aim = getATreeNode(path, oldname);

    if (aim == nullptr)
    {
        cout << "��path�����ڻ��path�²�������Ϊ" << oldname << "���ļ����ļ���!" << endl;
        cout << "������ʧ�ܣ�" << endl;
        return 1;
    }
    else
    {
        int retu = checkDupName(aim->parent, newname);
            if (retu == 0)//û�������ļ�
            {
                aim->name = newname;
                cout << "�������ɹ���" << endl;
              
            }
            else
            {
                cout << "��path�´��������ļ���\n������ʧ�ܣ�" << endl;
                
            }
            return retu;
    }
    
}

vector<string> FileSystem::search(string name)
{
    vector<string> ans;
    vector<TreeNode*>l;
    preOrderSearch(dirTree, name, l);
    if (l.size() == 0)
    {
        cout << "��Ϊ" << name << "���ļ����ļ��в����ڣ�" << endl;
    }
    else
    {

        cout << "����Ϊ������Ϊ" << name << "���ļ����ļ��е�·��:" << endl;
        for (int i = 0; i < l.size(); i++)
        {
            TreeNode* currentptr = l[i];
            string res;
            while (currentptr != dirTree)
            {
                res = currentptr->name + "\\" + res;
                currentptr = currentptr->parent;
            }
            res = currentptr->name + "\\" + res;
            res.pop_back();
            ans.push_back(res);
            cout << res << endl;
        }
    }
    return ans;
}


TreeNode*& FileSystem::matchPath(string path)//���� dirTree ����һ��path������ĩβ�Ľڵ�
{
    vector<string> res = split(path, "\\");
    TreeNode* temp = dirTree;
    TreeNode* lastptr = nullptr;

    int i;
    for (i = 0; i < res.size(); i++)
    {
        while (temp != nullptr)
        {
            if (temp->name == res[i] /*&& temp->type == 1*/)//�ڵ�ǰ���ҵ���
            {
                if (i < res.size() - 1)
                {
                    lastptr = temp;
                    temp = temp->child;//������һ���ƥ��
                }
                break;
            }
            else
            {
                lastptr = temp;
                temp = temp->sibling;
            }
        }
        if (temp == nullptr)
            break;
    }

    if (i == res.size())
    {
        cout << "·��ƥ��ɹ���" << endl;

        if (lastptr == nullptr)
            return dirTree;
        else if (lastptr->child == temp)
            return lastptr->child;
        else
            return lastptr->sibling;
    }
    else
    {
        cout << "�����ҵ�·��������" << endl;
        return temp;
    }
}

int FileSystem::create(string path,  string name,  int type, int size)//��path�´����ļ����ļ���name,��Ϊ֮����size�����ݿ�ռ�
{
    //�ҵ�����Ŀ¼�ڵ��λ��
    TreeNode * pnode=matchPath(path);
    
    if (pnode == nullptr)
    {
        cout << "��ǰĿ¼�����ڣ������½ڵ�ʧ�ܣ�\n" << endl;
        return 1;
    }
    if (pnode->type == 2)
    {
        cout << "�����ڷ�Ŀ¼�´����ڵ㣬�����½ڵ�ʧ�ܣ�" << endl;
        return 1;
    }
    TreeNode** aim = nullptr;
    if (pnode->child == nullptr)//��Ŀ¼�»�û�ж���ʱ
    {

       // pnode->child = new TreeNode(type, name, size, pnode, nullptr, nullptr);
        aim = &pnode->child;
       // applyForExternalStorage(*pnode->child);
    }
    else
    {
        TreeNode* currentptr = pnode->child;
        TreeNode* lastptr = nullptr;
        while (currentptr!=nullptr)
        {
            if (currentptr->name == name)
            {
                cout << "��ǰĿ¼�´���ͬ���ļ�/�ļ��У������½ڵ�ʧ�ܣ�\n" << endl;
                return 1;
            }
            lastptr = currentptr;
            currentptr = currentptr->sibling;
        }
        aim = &lastptr->sibling;
       /* lastptr->sibling= new TreeNode(type, name, size, pnode, nullptr, nullptr);
        applyForExternalStorage(*lastptr->sibling);*/
    }

    //����Ŀ¼�ڵ㲢�������
    *aim= new TreeNode(type, name, size, pnode, nullptr, nullptr);
    int ans = applyForExternalStorage(**aim);
    if (ans == 0)
    {
        cout << "�½ڵ㴴���ɹ���" << endl;
    }
    else
    {
        cout << "�½ڵ㴴��ʧ�ܣ�" << endl;
        delete *aim;
        *aim = nullptr;
    }
	return ans;
}

int FileSystem::applyForExternalStorage(FCB &fcb)
{
    int size = fcb.size;
    if (size < freeSpaceList.size())
    {
        fcb.location.insert(fcb.location.end(), freeSpaceList.begin(), freeSpaceList.begin() + size);
        freeSpaceList.erase(freeSpaceList.begin(), freeSpaceList.begin() + size);
        cout << "�������ռ�ɹ�!" << endl;
        return 0;
    }
    else
    {
        fcb.size = 0;
        cout << "�������ռ�ʧ�ܣ����ռ䲻�㣡" << endl;
        return 1;
    }
  
}

int FileSystem::releaseExternalStorage(FCB& fcb)
{
    int size = fcb.size;

    freeSpaceList.insert(freeSpaceList.end(),fcb.location.begin(),fcb.location.end());
  
    fcb.location.erase(fcb.location.begin(), fcb.location.end());
    return 0;
}

int FileSystem::checkDupName(TreeNode* t, string name)
{
    TreeNode* currentptr = t->child;
    while (currentptr!=NULL)
    {
        if (currentptr->name == name)//�������ļ�
        {
            return 1;
        }
        currentptr = currentptr->sibling;
    }
     return 0;
}

TreeNode* FileSystem::getATreeNode(string path, string name)
{
    TreeNode* pnode= matchPath(path);
    if (pnode != nullptr)
    {
        TreeNode* currentptr = pnode->child;
        while (currentptr != NULL)
        {
            if (currentptr->name == name)
            {
                return currentptr;
            }
            currentptr = currentptr->sibling;
        }
        return nullptr;
    }
    else
    {
        return nullptr;
    }
}

void FileSystem::printFreeSpaceList()
{
    cout << "\n���пռ��б�Ϊ:" << endl;
    for (int i = 0; i < freeSpaceList.size(); i++)
    {
        cout << freeSpaceList[i] << ' ';
    }
    cout << endl;
}

vector<string> FileSystem::split(const string& str, const string& delim)//�ַ�����Ƭ
{
    vector<string> res;
    if ("" == str) return res;
    //�Ƚ�Ҫ�и���ַ�����string����ת��Ϊchar*����  
    int len1 = str.length() + 1;//Ҫ����һ��\0
    char* strs = new char[len1]; 
    strcpy_s(strs,len1, str.c_str());

    int len2 = delim.length() + 1;
    char* d = new char[len2];
    strcpy_s(d, len2,delim.c_str());

    char* buf=NULL;
    char* p = strtok_s(strs, d,&buf);

    while (p)
    {
        string s = p; //�ָ�õ����ַ���ת��Ϊstring����  
        res.push_back(s); //����������  
        p = strtok_s(NULL, d,&buf);
    }

    delete [] strs;
    delete [] d;

    cout << "\n·��Ϊ��" << endl;
    for (int i = 0; i < res.size(); i++)
    {
        std::cout << res[i] << " ";
    }
    //cout << "�õ����ֶ�����" << res.size() << endl;
    return res;
}
