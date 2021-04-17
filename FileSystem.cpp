#include <iostream>
#include<vector>
#include<stack>
#include "FileSystem.h"
#include "FCB.h"

FileSystem::FileSystem()
{
    //空闲空间列表一开始全部空闲
    for (int i = 0; i < STORAGE_SIZE; i++)
    {
        freeSpaceList.push_back(i);
    }

    dirTree = new TreeNode(1, "root",0, nullptr, nullptr, nullptr);//创建一个叫root的文件夹
    swapAreaPath = "root\\temp";
    string temp=swapAreaPath.substr(5);
    create("root", temp, 1, 0);  //temp是用于存在中期调度进程的文件夹

    //创建一些初始文件和文件夹
    create("root", "chenshuyi", 1,0);
    create("root", "OS课设", 1,0);
    create("root\\OS课设", "version1.0",2,4);
    create("root\\chenshuyi", "hw", 1,0);
    create("root\\chenshuyi\\hw", "hw1", 1,0);
    create("root\\chenshuyi\\hw", "hw2", 2,5);
    create("root\\chenshuyi\\hw", "hw2", 2,5);  //测试用，证明不能在同一文件夹下创建同名文件或文件夹
    create("root\\chenshuyi\\hw", "hw3", 1,6);
    create("root\\chenshuyi\\hw\\hw1", "lalala", 2,10);
    create("root\\chenshuyi\\hw\\hw3", "lalala", 2, 1000);//测试用，证明外存不够时创建文件失败
    create("root\\chenshuyi\\hw\\hw3", "lalala", 2, 30);
    create("root\\chenshuyi\\hw\\hw2", "woc", 2,3); //测试用，证明不能在文件下创建文件
    create("root\\zhangwangting", "zwt", 2,9);      //测试用，证明不能在不存在的路径下创建文件
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

void FileSystem::postOrderDelSubTree(Tree &t)//后序遍历删除文件子树
{
    if (t == nullptr)
        return;

    postOrderDelSubTree(t->child);
    postOrderDelSubTree(t->sibling);
    //先删除数据块
    releaseExternalStorage(*t);
    //再删除目录项
    delete t;
    t = nullptr;
}

void FileSystem::preOrderTraverse(const Tree& t,int depth,vector<string> &s)//先序遍历文件目录树
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
    int ans=create("root\\temp", "pid" + to_string(pid), 2,size);//在文件树root\temp下创建一个叫pidxxx的文件
    if (ans == 0)
    {
        cout << "成功将进程" << pid << "的" << size << "块换到外存上！" << endl;
    }
    else
    {
        cout<<"没能将进程" << pid << "的" << size << "块换到外存上！" << endl;
    }
    return ans;
}

//图图、铭哥你们在调这个函数之前得保证有size块大小的空闲内存
int FileSystem::swapToMemory(int pid, int size)
{
   //删除相应目录节点，释放相应外存
    string path = swapAreaPath+"\\pid"+to_string(pid);
    int ans=del(path);
    return ans;
}

int FileSystem::printDir()//打印文件目录，即深搜打印树
{
    cout << "-------------------------------------------------------------------------------------------" << endl;
    cout << "文件目录如下：" << endl;
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
        cout << "删除节点失败，该路径不存在" << endl;
        return 1;
    }
    else
    {
        postOrderDelSubTree(pnode);
        cout << "删除子树成功！" << endl;
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
        cout << "该path不存在或该path下不存在名为" << oldname << "的文件或文件夹!" << endl;
        cout << "重命名失败！" << endl;
        return 1;
    }
    else
    {
        int retu = checkDupName(aim->parent, newname);
            if (retu == 0)//没有重名文件
            {
                aim->name = newname;
                cout << "重命名成功！" << endl;
              
            }
            else
            {
                cout << "该path下存在重名文件。\n重命名失败！" << endl;
                
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
        cout << "名为" << name << "的文件或文件夹不存在！" << endl;
    }
    else
    {

        cout << "以下为所有名为" << name << "的文件或文件夹的路径:" << endl;
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


TreeNode*& FileSystem::matchPath(string path)//在树 dirTree 上找一条path，返回末尾的节点
{
    vector<string> res = split(path, "\\");
    TreeNode* temp = dirTree;
    TreeNode* lastptr = nullptr;

    int i;
    for (i = 0; i < res.size(); i++)
    {
        while (temp != nullptr)
        {
            if (temp->name == res[i] /*&& temp->type == 1*/)//在当前层找到了
            {
                if (i < res.size() - 1)
                {
                    lastptr = temp;
                    temp = temp->child;//进入下一层的匹配
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
        cout << "路径匹配成功！" << endl;

        if (lastptr == nullptr)
            return dirTree;
        else if (lastptr->child == temp)
            return lastptr->child;
        else
            return lastptr->sibling;
    }
    else
    {
        cout << "待查找的路径不存在" << endl;
        return temp;
    }
}

int FileSystem::create(string path,  string name,  int type, int size)//在path下创建文件或文件夹name,并为之分配size块数据块空间
{
    //找到创建目录节点的位置
    TreeNode * pnode=matchPath(path);
    
    if (pnode == nullptr)
    {
        cout << "当前目录不存在，创建新节点失败！\n" << endl;
        return 1;
    }
    if (pnode->type == 2)
    {
        cout << "不能在非目录下创建节点，创建新节点失败！" << endl;
        return 1;
    }
    TreeNode** aim = nullptr;
    if (pnode->child == nullptr)//该目录下还没有东西时
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
                cout << "当前目录下存在同名文件/文件夹，创建新节点失败！\n" << endl;
                return 1;
            }
            lastptr = currentptr;
            currentptr = currentptr->sibling;
        }
        aim = &lastptr->sibling;
       /* lastptr->sibling= new TreeNode(type, name, size, pnode, nullptr, nullptr);
        applyForExternalStorage(*lastptr->sibling);*/
    }

    //创建目录节点并申请外存
    *aim= new TreeNode(type, name, size, pnode, nullptr, nullptr);
    int ans = applyForExternalStorage(**aim);
    if (ans == 0)
    {
        cout << "新节点创建成功！" << endl;
    }
    else
    {
        cout << "新节点创建失败！" << endl;
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
        cout << "申请外存空间成功!" << endl;
        return 0;
    }
    else
    {
        fcb.size = 0;
        cout << "申请外存空间失败，外存空间不足！" << endl;
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
        if (currentptr->name == name)//有重名文件
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
    cout << "\n空闲空间列表为:" << endl;
    for (int i = 0; i < freeSpaceList.size(); i++)
    {
        cout << freeSpaceList[i] << ' ';
    }
    cout << endl;
}

vector<string> FileSystem::split(const string& str, const string& delim)//字符串切片
{
    vector<string> res;
    if ("" == str) return res;
    //先将要切割的字符串从string类型转换为char*类型  
    int len1 = str.length() + 1;//要加上一个\0
    char* strs = new char[len1]; 
    strcpy_s(strs,len1, str.c_str());

    int len2 = delim.length() + 1;
    char* d = new char[len2];
    strcpy_s(d, len2,delim.c_str());

    char* buf=NULL;
    char* p = strtok_s(strs, d,&buf);

    while (p)
    {
        string s = p; //分割得到的字符串转换为string类型  
        res.push_back(s); //存入结果数组  
        p = strtok_s(NULL, d,&buf);
    }

    delete [] strs;
    delete [] d;

    cout << "\n路径为：" << endl;
    for (int i = 0; i < res.size(); i++)
    {
        std::cout << res[i] << " ";
    }
    //cout << "得到的字段数：" << res.size() << endl;
    return res;
}
