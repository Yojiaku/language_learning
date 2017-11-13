#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    char const *envp[] = {"AA=11", "BB=22", NULL};
    printf("进入主函数main……\n");
    int ret; // 返回值
    ret = execle("./hello", "hello", NULL, envp); // 执行当前目录下的hello可执行文件
    if(ret == -1)
        perror("execle 错误！\n");
    printf("退出主函数……\n");
    return 0;
}
