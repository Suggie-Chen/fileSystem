#pragma once
#include<string>
using namespace std;
//struct FCB
//{
//	char type;//1Ϊ�ļ���2Ϊ�ļ���
//	string name;
//	FCB* parent;
//	FCB* child;
//	FCB* sibling;
//};

class FCB
{
public:
	FCB(char newtype,string newname,FCB* newparent,FCB* newchild,FCB* newsibling);
	char type;//1Ϊ�ļ���2Ϊ�ļ���
	string name;
	FCB* parent;
	FCB* child;
	FCB* sibling;

};

typedef FCB TreeNode;
typedef FCB* Tree;



