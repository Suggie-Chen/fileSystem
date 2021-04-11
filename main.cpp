#pragma warning( disable : 4996)
#include<iostream>
#include<vector>
#include "fileSystemLib.h"
#include"FileSystem.h"


int main()
{
 
    FileSystem f;
    f.create("root\\chenshuyi\\linux", "hw", 1);
    return 0;
}