#pragma warning( disable : 4996)
#pragma once
#include<vector>
#include "fileSystemLib.h"

class FileSystem
{
public:
	FileSystem();

	int del(string path, string filename);//ɾ��path�µ��ļ�filename
	int show(string path, string filename);//��ʾpath�µ��ļ�filename������
	int edit(string path, string filename);//�༭path�µ��ļ�filename
	int find(string path, string name);//��path���ҽ�name���ļ����ļ���

	TreeNode* matchPath(string path, const Tree& t);
	int create(const Tree& t, string path, string name, int type);//��path�´����ļ����ļ���name

	vector<string> split(const string& str, const string& delim);//�ַ�����Ƭ

private:
	FCB* root;
	
};

