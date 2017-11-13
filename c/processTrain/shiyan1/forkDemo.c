#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
int main()
{
    pid_t pid = fork();
    if(pid == 0) printf("Yes\n");
    return 0;
}
