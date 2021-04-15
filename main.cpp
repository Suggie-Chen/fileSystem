
#include<iostream>
#include<vector>
//#include "fileSystemLib.h"
#include"FileSystem.h"


int main()
{
 
    FileSystem f;
    f.create("root", "chenshuyi", 1);//意思是在root文件夹下创建一个叫hw的文件夹
    f.create("root\\chenshuyi", "hw", 1);
    f.create("root\\chenshuyi\\hw", "hw1", 1);
    f.create("root\\chenshuyi\\hw", "hw2", 2);
    f.create("root\\chenshuyi\\hw", "hw2", 2);
    f.create("root\\chenshuyi\\hw", "hw3", 2);
    f.create("root\\chenshuyi\\hw\\hw1", "lalala", 2);
    f.printDir();
   // f.create("root\\zhangwangting", "zwt", 2);
    f.del("root\\chenshuyi","hw");
    f.printDir();
    return 0;
}

