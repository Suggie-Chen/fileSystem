#pragma warning( disable : 4996)
#pragma once
#include<vector>
#include "FCB.h"

class FileSystem
{
public:
	FileSystem();
	//~FileSystem();//new �˺ܶ����ڵ�

	void postOrderDelSubTree(Tree &t);
	void preOrderTraverse(const Tree& t, int depth, vector<string>& s);
	int del(string path,string subtreeroot);//ɾ��path�����µ����нڵ�
	int printDir();
	int show(string path, string filename);//��ʾpath�µ��ļ�filename������
	int edit(string path, string filename);//�༭path�µ��ļ�filename
	int create(string path, string name, int type);//��path�´����ļ����ļ���name
	TreeNode* matchPath(string path, const Tree& t);

	vector<string> split(const string& str, const string& delim);//�ַ�����Ƭ

private:
	Tree dirTree;
	
};

