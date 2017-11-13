/*
 *测试在子进程中使用exit，是否会直接退出整个程序
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

void childFunc();

void childFunc()
{
    printf("这是子进程，他的PID是%d, 他的父进程PID是%d.\n", getpid(), getppid());
    exit(0);
}

int main()
{
    printf("这是main函数，也就是1号进程，他的PID是%d，他的父进程PID是%d.\n", getpid(), getppid());
    // 创建一个子进程
    pid_t child;
    child = fork();
    if(child == 0){
        // 说明创建子进程成功
        // 子进程执行一个任务
        childFunc(); 
    }else if(child < 0){
        puts("进程创建失败\n");
        exit(1);
    }else if(child > 0){
        printf("真的会输出吗？\n");
        printf("如果输出了的话就打印PID瞧瞧是哪个进程！\n");
        printf("现在这个进程PID是%d.\n", getpid());
    }
    /*
     发现一个有趣的问题：若直接是else，而不是else if(child < 0)，那么else里面的语句依旧会被输出，后来找到原因：
     这是逻辑有问题，直接else的话，程序不知道这是在说child的其他值。
     ！！！
     NO，测试了else if(child > 0)，发现里面的语句还是会输出！所以这并不是逻辑出错了，if else语句是知道指的是child
     这个变量的！
     哦哦！原来fork()是个神奇的函数！他仅仅被调用了一次，却能返回两次，更多见注释【2】fork
    */
    // 父进程等待子进程退出
    pid_t cpid = wait(NULL);
    printf("子进程%d已经退出.\n", cpid);
    // 父进程自己退出
    printf("父进程%d退出，程序终止\n", getpid());
    return 0;
}
