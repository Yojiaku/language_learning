#include <stdio.h>
#include <unistd.h>
int main()
{
	pid_t pid = fork();
	if(pid == 0) // 说明是子进程
	{
		//puts("Hi, I am a child process");
		fputs("Hi, I am a child process", stdout);
	}
	else  // 说明是父进程
	{
		//puts("My child process's id is :%d \n", pid);
		printf("My child process's id is %d \n", pid);
		sleep(5);
	}

	if(pid == 0) puts("End child process");
	else puts("End parent process");

	return 0;
}
