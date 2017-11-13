#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h> // exit()
int main()
{
	pid_t pid; // 进程号
	char *message;
	int n;

	printf("fork program starting\n");
	pid = fork(); // <==> pid_t fork(void) 包含在头文件<sys/types.h>和<unistd.h>中
	switch(pid) // 进程会根据pid不同，有选择地执行各自的代码
	{
		case -1:
			perror("fork failed");
			exit(1);
		case 0: // fork函数返回新进程的pid，新进程为0（即子进程）
			message = "This is the childprocess";
			n = 5;
			break;
		default:
			message = "This is the parentprocess";
			n = 3;
			break;
	}

	for(; n>0; n--){
		puts(message);;
		sleep(1);
	}
	exit(0);
}
