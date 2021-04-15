#pragma warning( disable : 4996)
#pragma once
#include<vector>
#include "FCB.h"

class FileSystem
{
public:
	FileSystem();
	//~FileSystem();//new 了很多树节点

	void postOrderDelSubTree(Tree &t);
	void preOrderTraverse(const Tree& t, int depth, vector<string>& s);
	int del(string path,string subtreeroot);//删除path及以下的所有节点
	int printDir();
	int show(string path, string filename);//显示path下的文件filename的内容
	int edit(string path, string filename);//编辑path下的文件filename
	int create(string path, string name, int type);//在path下创建文件或文件夹name
	TreeNode* matchPath(string path, const Tree& t);

	vector<string> split(const string& str, const string& delim);//字符串切片

private:
	Tree dirTree;
	
};

