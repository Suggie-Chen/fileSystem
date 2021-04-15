#pragma once
#include<string>
using namespace std;
//struct FCB
//{
//	char type;//1为文件，2为文件夹
//	string name;
//	FCB* parent;
//	FCB* child;
//	FCB* sibling;
//};

class FCB
{
public:
	FCB(char newtype,string newname,FCB* newparent,FCB* newchild,FCB* newsibling);
	char type;//1为文件，2为文件夹
	string name;
	FCB* parent;
	FCB* child;
	FCB* sibling;

};

typedef FCB TreeNode;
typedef FCB* Tree;



