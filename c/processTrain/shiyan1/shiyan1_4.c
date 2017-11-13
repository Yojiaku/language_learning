#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

typedef enum{false, true}bool;
bool isPrime(int);
void t1_prime();
void t2_fibonacci();

int main()
{
    printf("Hello, this is No.1 process, pid is %d, and its ppid is %d.\n", getpid(), getppid());
    
    pid_t child2 = fork();
    if(child2 < 0)
        printf("No.1 process failed to create No.2 childprocess.\n");
    else if(child2 == 0)
    {   
        printf("Hello, this is No.2 process, pid is %d, and its ppid is %d.\n", getpid(), getppid());
        execl("/bin/ps", "ps", "-au", NULL);
        exit(0);
    }
    pid_t cpid2 = wait(NULL);
    printf("The process %d exits.\n", cpid2);
    
    pid_t child3 = fork();
    if(child3 < 0) printf("No.1 process failed to create No.3 childprocess.\n");
    else if(child3 == 0)
    {
        printf("Hello, this is No.3 process, pid is %d, and its ppid is %d.\n", getpid(), getppid());
        pid_t child4 = fork();
        if(child4 < 0) printf("No.3 process failed to create No.4 childprocess.\n");
        else if(child4 == 0)
        {
            printf("Hello, this is No.4 process, pid is %d, and its ppid is %d.\n", getpid(), getppid());
            int ret1 = 0, ret2 = 0;
            pthread_t id1, id2;
            ret1 = pthread_create(&id1, NULL, (void*)t1_prime, NULL);
            if(ret1 != 0) printf("Failed to create No.1 thread.\n");
            ret1 = pthread_join(id1, NULL);
            if(ret1 == 0) printf("No.1 thread exited.\n");
            ret2 = pthread_create(&id2, NULL, (void*)t2_fibonacci, NULL);
            if(ret2 != 0) printf("Failed to create No.2 thread.\n"); 
            ret2 = pthread_join(id2, NULL);
            if(ret2 == 0) printf("No.2 thread exited.\n");
            exit(0);
        }    
        pid_t cpid4 = wait(NULL);
        printf("The process %d exited.\n", cpid4);

        pid_t child5 = fork();
        if(child5 < 0) printf("No.3 process failed to create No.5 childprocess.\n");
        else if(child5 == 0)
        {
            printf("Hello, this is No.5 process, pid is %d, and its ppid is %d.\n", getpid(), getppid());
            char const *envp[] = {"AA=11", "BB=22", NULL};
            execle("./hello", "hello", NULL, envp);
            exit(0);
        }
        
        pid_t cpid5 = wait(NULL);
        printf("The process %d exited.\n", cpid5);
        printf("Parent process(No.3) %d exits.\n", getpid());
        exit(0);
    }
    pid_t cpid3 = wait(NULL);
    printf("The process %d exits.\n", cpid3);
    printf("Parent process %d exits.\n", getpid());

    return 0;
}

void t1_prime()
{
    printf("Hello, I am a thread, my task is to find the prime numbers between 1 and n, my tid is %lu.\n", pthread_self());
    int n = 10;
    printf("enter the n for prime number:\n");
    scanf("%d", &n);
    for(int i=0; i<=n; i++) 
        if(isPrime(i)) printf("%d, ", i);   
    printf("\n");
    pthread_exit(0);
}

bool isPrime(int n)
{
    if(n < 2) return false;
    for(int i=2; i<=n/2; i++)
        if(n%i == 0)
             return false;
    return true;
}

void t2_fibonacci()
{
    printf("Hello, I am a thread, my task is to find the Fibonacci between 1 and n, my tid is %lu.\n", pthread_self());
    int n = 10;
    printf("enter the n for fibonacci:\n");
    scanf("%d", &n);
    int a, b, c;
    a = 0;
    b = 1;
    c = 0;
    while(c <= n)
    {
        if(c >= 1)
            printf("%d, ", c);
        a = b;
        b = c;
        c = a+b;
    }
    printf("\n");
    pthread_exit(0);
}
