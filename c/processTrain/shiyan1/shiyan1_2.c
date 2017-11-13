#include <sys/types.h> // for pid_t
#include <unistd.h> // for exec...
#include <stdio.h>
#include <stdlib.h>  // for [void exit(int status)]
#include <sys/wait.h> // for wait()
#include <pthread.h> // for thread

typedef enum{false, true}bool;
int createProcess();
void chooseTask(int);
int createThread();
void *MyThread1(); // 线程1求1～n之间的素数
void *MyThread2(); // 线程2求fbi序列
bool isPrime(int);

bool isPrime(int n)
{
    if(n < 2) return false;

    for(int i=2; i<=n/2; i++)
    {
        if(n%i == 0) return false;
    }
    return true;
}

void *MyThread1()
{
   printf("Entering No.1 thread, prime number...\n");
   int start = 1;
   int end = 10; // 求1~100的素数
   printf("The Prime Number between %d and %d is:\n", start, end);
   for(int i=start; i<=end; i++)
       if(isPrime(i)) printf("%d, ", i);
   printf("\n");
   return 0;  
}

void *MyThread2()
{
   printf("Entering No.2 thread, Fibonacci...\n");
   int a, b, c;
   int start = 0;
   int end = 10;
   a = 0;
   b = 1;
   c = 0;
   printf("Fibonacci terms:\n");
   while(c <= end)
   {
       if(c >= start)
           printf("%d, ", c);
       a = b; // copy n-1 to n-2
       b = c; // copy current to n-1
       c = a+b; // new term
   }
   printf("\n");
   return 0;
}

int createProcess()
{
   int i;
   for(i=4; i<=5; i++)
   {
       pid_t child;
       child = fork();
       if(child < 0)
       {
           puts("Error happend in fork()!");
           //exit(1);
           return 1;
       }else if(child == 0){
           chooseTask(i);
       }
    }
    for(i=4; i<=5; i++)
    {
		// 父进程等待子进程退出
		pid_t cpid = wait(NULL);
		printf("No.%d process, its PID is %d exits\n", i, cpid);
    }
    // 最后父进程退出
    printf("No.3 process(also the parent of No.4 and No.5), PID is %d exits\n", getpid());
    return 0;
}

int createThread()
{
    int ret1 = 0, ret2 = 0;
    pthread_t id1, id2; // 定义两个线程的id
    // 调用pthread_create()创建线程1
    ret1 = pthread_create(&id1, NULL, (void*)MyThread1, NULL);
    // ret1 = 0表示创建成功，否则表示出错
    if(ret1 != 0)
    {
        printf("Create pthread error!\n");
        //exit(1);
        return 1;
    }
    // 调用pthread_create()创建线程2
    ret2 = pthread_create(&id2, NULL, (void*)MyThread2, NULL);
    // ret2 = 0表示创建成功，否则表示出错
    if(ret2 != 0)
    {
        printf("Create pthread error!\n");
        //exit(1);
        return 1;
    }
    pthread_join(id1, NULL); // 主线程等待线程1结束
    pthread_join(id2, NULL); // 主线程等待线程2结束
    printf("main thread exit!\n"); // 主线程退出
    return 0;
}

void chooseTask(int processNo)
{
   switch(processNo)
   {  
	case 2: // 表示2号进程，执行系统调用命令ps,同时打印出其pid和父进程pid
	    printf("Hello, this is No.2 process, its pid is %d, and its parent's pid is %d.\n", getpid(), getppid());
            // ps命令
	    execl("/bin/ps", "ps", "-au", NULL);
	    break;
	case 3: // 表示3号进程，创建4\5两个进程同时打印出其pid和父进程pid
	    printf("Hello, this is No.3 process, its pid is %d, and its parent's pid is %d.\n", getpid(), getppid());
        int retPro;
	    retPro = createProcess();
        if(retPro == 0)
            printf("No.3 process, PID is %d, successfully created 2 processes!\n", getpid());
        else if(retPro == 1)
            printf("No.3 process, PID is %d, failed to create process.\n", getpid()); 
        break;
    case 4: // 表示4号进程，创建两个线程Thread1，Thread2
        printf("Hello, this is No.4 process, its pid is %d, and its parent's pid is %d.\n", getpid(), getppid());
        int retThr;
        retThr = createThread();
        if(retThr == 0)
        	printf("No.4 process, PID is %d, successfully created 2 threads!\n", getpid());
        else if(retThr == 1)
        	printf("No.4 process, PID is %d, failed to create thread.\n", getpid());
        break;
    case 5: // 表示5号进程，执行一个用户编写的可执行文件
        printf("Hello, this is No.5 process, its pid is %d, and its parent's pid is %d.\n", getpid(), getppid());
        char const *envp[] = {"AA=11", "BB=22", NULL};
        printf("Entering No.5 process...\n");
        int ret;
        ret = execle("./hello", "hello", NULL, envp); // 执行当前目录下的hello可执行文件
        if(ret == -1)
            perror("execle 错误！\n");
        printf("exit No.5 process...\n");  // 不出意外，这句不会被输出
        break;
   }
   exit(0);
}


int main()
{
    // 1号进程就是main函数，现在获取这个main函数的pid，并打印出来
    // 1号进程的父进程就是bash（shell）
    printf("Hello, this is No.1 process, its pid is %d, and its parent's pid is %d.\n", getpid(), getppid());
    // 1号进程创建2、3号进程
    int i;
    for(i=2; i<=3; i++)
    {
		pid_t child;
		child = fork(); // 调用fork()创建子进程
		if(child < 0)
		{
		   // 若进程创建失败
		   puts("Error happened in fork()!");
	           // 这里使用exit(1)退出感觉不是很好的方法，因为使用后便强行退出了之后任何代码都不会有执行的机会
		   return 0; 
		}else if(child == 0){
		   // 子进程创建成功
		   // 每个子进程执行不同的任务
		   chooseTask(i);
		}else{
			// 这是父进程
			printf("No.1 process, its pid is %d, and it created a process, its pid is %d.\n", getpid(), child);
		}    
    }

    for(i=0; i<2; i++)
    {
	// 父进程等待子进程退出
	pid_t cpid = wait(NULL);
	printf("The process %d exits\n", cpid);
    }
    // 最后父进程退出
    printf("Parent process's ID is %d exits\n", getpid());
    return 0;
}
