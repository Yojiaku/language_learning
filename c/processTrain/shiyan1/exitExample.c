/*
 测试exit函数与return函数的不同
*/
#include <stdio.h>
#include <stdlib.h>

void exitExample();

void exitExample()
{
    int i = 0;
    if(i==0)
        exit(0);
}

int main()
{
    // 调用一个函数，测试其用exit是否还会返回到main函数
    exitExample();
    printf("看来在调用的函数中使用exit也能返回到调用这个函数的父函数来.\n");
    return 0;
}
