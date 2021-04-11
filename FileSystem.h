#pragma warning( disable : 4996)
#pragma once
#include<vector>
#include "fileSystemLib.h"

class FileSystem
{
public:
	FileSystem();

	int del(string path, string filename);//删除path下的文件filename
	int show(string path, string filename);//显示path下的文件filename的内容
	int edit(string path, string filename);//编辑path下的文件filename
	int find(string path, string name);//在path下找叫name的文件或文件夹

	TreeNode* matchPath(string path, const Tree& t);
	int create(const Tree& t, string path, string name, int type);//在path下创建文件或文件夹name

	vector<string> split(const string& str, const string& delim);//字符串切片

private:
	FCB* root;
	
};

